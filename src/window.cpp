/*
  Copyright 2009  Graeme Gott <graeme@gottcode.org>
  Copyright 2010  Ronny Yabar Aizcorbe <ronnycontacto@gmail.com>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of
  the License or (at your option) version 3 or any later version
  accepted by the membership of KDE e.V. (or its successor approved
  by the membership of KDE e.V.), which shall act as a proxy
  defined in Section 14 of version 3 of the license.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "window.h"
#include "board.h"
#include "settings.h"

#include <QAction>
#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QStatusBar>
#include <QUndoStack>
#include <QVBoxLayout>

#include <KActionCollection>
#include <KStandardGameAction>
#include <KToggleAction>
#include <KComboBox>
#include <KConfigDialog>
#include <KConfigGroup>
#include <KScoreDialog>
#include <KgThemeSelector>
#include <KGameClock>
#include <KLocale>
#include <KMessageBox>
#include <KSharedConfig>


#include <QFormLayout>
#include <ctime>

KpegMainWindow::KpegMainWindow()
{
    m_difficulty = 5;
    m_algorithm = 1;
    m_moves = new QUndoStack(this);
    m_gameClock = new KGameClock(this, KGameClock::MinSecOnly);
    connect(m_gameClock, SIGNAL(timeChanged(const QString&)), SLOT(updateTimer(const QString&)));

    m_board = new Board(m_moves, this);
    connect(m_board, SIGNAL(countChanged(int)), SLOT(updateMoves(int)));

    setCentralWidget(m_board);

    m_statusBar = statusBar();
    m_levelLabel->setText(i18n("Algorithm: %1", QLatin1String("")));
    m_movesLabel->setText(i18n("Moves: %1", QLatin1String("0")));
    m_timeLabel->setText(i18n("Time: %1", QLatin1String("00:00")));

    m_statusBar->addPermanentWidget(m_levelLabel, 1);
    m_statusBar->addPermanentWidget(m_movesLabel, 2);
    m_statusBar->addPermanentWidget(m_timeLabel, 3);

    setupActions();
    setupGUI();

    startGame(m_algorithm);
}

KpegMainWindow::~KpegMainWindow()
{
    // Save current game
    if (m_moves->count()) {
        QStringList moves;
        int count = m_moves->index();
        for (int i = 0; i < count; ++i) {
            moves += m_moves->text(i);
        }
        
        KConfig *config = KSharedConfig::openConfig().data();
        KConfigGroup savegame = config->group("Game");
        savegame.writeEntry("Moves", moves);
    }
}

void KpegMainWindow::setupActions()
{
    KStandardGameAction::gameNew(this, SLOT(newGame()), actionCollection());
    KStandardGameAction::restart(this, SLOT(restartGame()), actionCollection());
    m_actionPause = KStandardGameAction::pause(this, SLOT(pauseGame(bool)), actionCollection());
    KStandardGameAction::highscores(this, SLOT(showHighscores()), actionCollection());
    KStandardGameAction::quit(this, SLOT(close()), actionCollection());
    KStandardAction::preferences(this, SLOT(configureSettings()), actionCollection());

    QAction* undoAction = KStandardGameAction::undo(this, SLOT(undoMove()), actionCollection());
    undoAction->setEnabled(false);
    connect(m_moves, SIGNAL(canUndoChanged(bool)), undoAction, SLOT(setEnabled(bool)));

    QAction* redoAction = KStandardGameAction::redo(this, SLOT(redoMove()), actionCollection());
    redoAction->setEnabled(false);
    connect(m_moves, SIGNAL(canRedoChanged(bool)), redoAction, SLOT(setEnabled(bool)));
    
    m_playSoundsAction = new KToggleAction(i18n("&Play Sounds"), this);
    actionCollection()->addAction(QLatin1String("play_sounds"), m_playSoundsAction);
    m_playSoundsAction->setChecked(KpegSettings::playSounds());
    connect(m_playSoundsAction, SIGNAL(triggered(bool)), this, SLOT(setSounds(bool)));
    
    Kg::difficulty()->addStandardLevelRange(
        KgDifficultyLevel::Easy, KgDifficultyLevel::VeryHard
    );

    KgDifficultyGUI::init(this);
    connect(Kg::difficulty(), SIGNAL(currentLevelChanged(const KgDifficultyLevel*)), SLOT(levelChanged()));
}

void KpegMainWindow::newGame()
{
    m_gameClock->restart();
    m_gameClock->pause();

    if (m_actionPause->isChecked()) {
        m_board->setGamePaused(false);
        m_actionPause->setChecked(false);
    }
    m_actionPause->setEnabled(false);
    Kg::difficulty()->setGameRunning(false);

    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle(i18n("New"));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    dialog->setLayout(mainLayout);
    mainLayout->addWidget(mainWidget);

    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    dialog->connect(buttonBox, SIGNAL(accepted()), SLOT(accept()));
    dialog->connect(buttonBox, SIGNAL(rejected()), SLOT(reject()));
    mainLayout->addWidget(buttonBox);

    // Create contents of dialog
    QWidget* contents = new QWidget(dialog);
    mainLayout->addWidget(contents);

    KComboBox* algorithms_box = new KComboBox(contents);
    mainLayout->addWidget(algorithms_box);
    algorithms_box->addItem(i18n("Original"), 1);
    algorithms_box->addItem(i18n("Branch"), 2);
    algorithms_box->addItem(i18n("Line"), 3);
    algorithms_box->setCurrentIndex(algorithms_box->findData(m_algorithm));

    // Layout dialog
    QFormLayout* box_layout = new QFormLayout(contents);
    box_layout->addRow(i18n("Algorithm:"), algorithms_box);

    if (dialog->exec() == QDialog::Accepted) {
        startGame(algorithms_box->itemData(algorithms_box->currentIndex()).toInt());
    }

    delete dialog;
    updateMoves(0);
    updateTimer(QLatin1String("00:00"));
}

void KpegMainWindow::restartGame()
{
    if (m_board->isFinished() || KMessageBox::questionYesNo(0, i18n("Do you want to restart?")) == KMessageBox::Yes) {
        m_board->generate(m_difficulty, m_algorithm);
    }
    m_gameClock->restart();
    m_gameClock->resume();
    updateMoves(0);
}

void KpegMainWindow::configureSettings()
{
    if (KConfigDialog::showDialog(QLatin1String("settings"))) {
        return;
    }
    KConfigDialog* dialog = new KConfigDialog(this, QLatin1String("settings"), KpegSettings::self());
    dialog->addPage(new KgThemeSelector(m_board->renderer()->themeProvider()), i18n("Theme"), QLatin1String("games-config-theme"));
    connect(m_board->renderer()->themeProvider(), SIGNAL(currentThemeChanged(const KgTheme*)), SLOT(loadSettings()));
    connect(dialog, SIGNAL(settingsChanged(const QString&)), this, SLOT(loadSettings()));
    dialog->show();
}

void KpegMainWindow::loadSettings()
{
    m_board->setTheme();
    KConfig *config = KSharedConfig::openConfig().data();
    KConfigGroup savegame = config->group("Game");
}

void KpegMainWindow::startGame(int algorithm)
{
    m_actionPause->setEnabled(true);
    m_gameClock->resume();
    Kg::difficulty()->setGameRunning(true);

    switch (Kg::difficultyLevel()) {
    default:
    case KgDifficultyLevel::Easy:
        m_difficulty = 15;
        break;
    case KgDifficultyLevel::Medium:
        m_difficulty = 30;
        break;
    case KgDifficultyLevel::Hard:
        m_difficulty = 40;
        break;
    case KgDifficultyLevel::VeryHard:
        m_difficulty = 60;
        break;
    }

    m_algorithm = algorithm;
    QString text;
    
    switch (m_algorithm) {
    default:
    case 1:
        text = i18n("Original");
        break;
    case 2:
        text = i18n("Branch");
        break;
    case 3:
        text = i18n("Line");
        break;
    }

    m_levelLabel->setText(i18n("Algorithm: %1", text));
    m_board->generate(m_difficulty, m_algorithm);

    KConfig *config = KSharedConfig::openConfig().data();
    KConfigGroup savegame = config->group("Game");
    savegame.writeEntry("Difficulty", m_difficulty);
    savegame.writeEntry("Algorithm", m_algorithm);
    savegame.writeEntry("Moves", QStringList());
}

void KpegMainWindow::updateMoves(int count)
{
    m_movesLabel->setText(i18n("Moves: %1", count));
}

void KpegMainWindow::pauseGame(bool paused)
{
    m_board->setGamePaused(paused);
    if (paused) {
        m_gameClock->pause();
    } else {
        m_gameClock->resume();
    }
}

void KpegMainWindow::updateTimer(const QString& timeStr)
{
    m_timeLabel->setText(i18n("Time: %1", timeStr));
}

void KpegMainWindow::levelChanged()
{
    KpegSettings::self()->save();
    startGame(m_algorithm);
}

void KpegMainWindow::setSounds(bool enable)
{
    KpegSettings::setPlaySounds(enable);
    KpegSettings::self()->save();
}

void KpegMainWindow::showHighscores()
{
    KScoreDialog ksdialog(KScoreDialog::Name | KScoreDialog::Level | KScoreDialog::Time, this);
    ksdialog.setConfigGroup(qMakePair(
        Kg::difficulty()->currentLevel()->key(),
        Kg::difficulty()->currentLevel()->title()
    ));
    ksdialog.exec();
}

void KpegMainWindow::undoMove()
{
    m_moves->undo();
    m_board->updateMovesCounter(true);
}

void KpegMainWindow::redoMove()
{
    m_moves->redo();
    m_board->updateMovesCounter(false);
}
