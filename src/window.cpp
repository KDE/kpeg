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

#include <KApplication>
#include <KAction>
#include <KActionCollection>
#include <KStandardGameAction>
#include <KToggleAction>
#include <KComboBox>
#include <KConfigDialog>
#include <KConfigGroup>
#include <KDialog>
#include <KScoreDialog>
#include <KGameThemeSelector>
#include <KGameClock>
#include <KGlobal>
#include <KLocale>
#include <KMessageBox>
#include <KSharedConfig>
#include <KStatusBar>
#include <KUndoStack>

#include <QFormLayout>
#include <ctime>

KpegMainWindow::KpegMainWindow(QWidget* parent)
        : KXmlGuiWindow(parent),
        m_difficulty(5),
        m_algorithm(1)
{
    m_moves = new KUndoStack(this);
    m_gameClock = new KGameClock(this, KGameClock::MinSecOnly);
    connect(m_gameClock, SIGNAL(timeChanged(const QString&)), SLOT(updateTimer(const QString&)));

    m_board = new Board(m_moves, this);
    connect(m_board, SIGNAL(countChanged(int)), SLOT(updateMoves(int)));

    setCentralWidget(m_board);

    statusBar()->insertItem(i18n("Algorithm: "), 0);
    statusBar()->insertItem(i18n("Moves: 0"), 1);
    statusBar()->insertItem(i18n("Time: 00:00"), 2);

    setupActions();
    setupGUI();

    KGameDifficulty::setLevel(KGameDifficulty::Easy);
    loadGame();

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
        KConfigGroup savegame = KGlobal::config()->group("Game");
        savegame.writeEntry("Moves", moves);
    }
}

void KpegMainWindow::setupActions()
{
    KStandardGameAction::gameNew(this, SLOT(newGame()), actionCollection());
    KStandardGameAction::restart(this, SLOT(restartGame()), actionCollection());
    m_actionPause = KStandardGameAction::pause(this, SLOT(pauseGame(bool)), actionCollection());
    KStandardGameAction::highscores(this, SLOT(showHighscores()), actionCollection());
    KStandardGameAction::quit(kapp, SLOT(quit()), actionCollection());
    KStandardAction::preferences(this, SLOT(configureSettings()), actionCollection());

    KAction* action = KStandardGameAction::undo(m_moves, SLOT(undo()), actionCollection());
    action->setEnabled(false);
    connect(m_moves, SIGNAL(canUndoChanged(bool)), action, SLOT(setEnabled(bool)));

    action = KStandardGameAction::redo(m_moves, SLOT(redo()), actionCollection());
    action->setEnabled(false);
    connect(m_moves, SIGNAL(canRedoChanged(bool)), action, SLOT(setEnabled(bool)));

    KGameDifficulty::init(this, this, SLOT(levelChanged(KGameDifficulty::standardLevel)));
    KGameDifficulty::setRestartOnChange(KGameDifficulty::RestartOnChange);
    KGameDifficulty::addStandardLevel(KGameDifficulty::VeryEasy);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Easy);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Medium);
    KGameDifficulty::addStandardLevel(KGameDifficulty::Hard);
    KGameDifficulty::addStandardLevel(KGameDifficulty::VeryHard);
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
    KGameDifficulty::setRunning(false);

    KDialog* dialog = new KDialog(this);
    dialog->setCaption(i18n("New"));
    dialog->setButtons(KDialog::Ok | KDialog::Cancel);

    // Create contents of dialog
    QWidget* contents = new QWidget(dialog);
    dialog->setMainWidget(contents);

    KComboBox* algorithms_box = new KComboBox(contents);
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
    statusBar()->changeItem(i18n("Time: 00:00"), 2);
}

void KpegMainWindow::restartGame()
{
    if (m_board->isFinished() || KMessageBox::questionYesNo(0, i18n("Do you want to restart?")) == KMessageBox::Yes) {
        m_board->generate(m_difficulty, m_algorithm);
    }
    m_gameClock->restart();
    m_gameClock->resume();
}

void KpegMainWindow::configureSettings()
{
    if (KConfigDialog::showDialog("settings")) {
        return;
    }
    KConfigDialog* dialog = new KConfigDialog(this, "settings", KpegSettings::self());
    dialog->addPage(new KGameThemeSelector(dialog, KpegSettings::self(), KGameThemeSelector::NewStuffDisableDownload), i18n("Theme"), "games-config-theme");
    connect(dialog, SIGNAL(settingsChanged(const QString&)), this, SLOT(loadSettings()));
    dialog->show();
}

void KpegMainWindow::loadSettings()
{
    m_board->setTheme(KpegSettings::theme());
    KConfigGroup savegame = KGlobal::config()->group("Game");
}

void KpegMainWindow::loadGame()
{
    // Load board
    KConfigGroup savegame = KGlobal::config()->group("Game");
    QStringList moves = savegame.readEntry("Moves", QStringList());

    startGame(m_algorithm);

    // Load moves
    QRegExp parse("(-?\\d+)x(-?\\d+):(-?\\d+)x(-?\\d+)");
    foreach(const QString& move, moves) {
        if (!parse.exactMatch(move)) {
            continue;
        }

        QPoint old_hole(parse.cap(1).toInt(), parse.cap(2).toInt());
        QPoint new_hole(parse.cap(3).toInt(), parse.cap(4).toInt());
        if (m_board->isPeg(old_hole) && m_board->isHole(new_hole)) {
            m_board->move(old_hole, new_hole);
        } else {
            qWarning("Invalid move: %dx%d to %dx%d", old_hole.x(), old_hole.y(), new_hole.x(), new_hole.y());
        }
    }
}

void KpegMainWindow::startGame(int algorithm)
{
    m_actionPause->setEnabled(true);
    m_gameClock->resume();
    KGameDifficulty::setRunning(true);

    switch (KGameDifficulty::level()) {
    default:
    case KGameDifficulty::VeryEasy:
        m_difficulty = 5;
        break;
    case KGameDifficulty::Easy:
        m_difficulty = 15;
        break;
    case KGameDifficulty::Medium:
        m_difficulty = 30;
        break;
    case KGameDifficulty::Hard:
        m_difficulty = 40;
        break;
    case KGameDifficulty::VeryHard:
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

    statusBar()->changeItem(i18n("Algorithm: %1", text), 0);

    m_board->generate(m_difficulty, m_algorithm);

    KConfigGroup savegame = KGlobal::config()->group("Game");
    savegame.writeEntry("Difficulty", m_difficulty);
    savegame.writeEntry("Algorithm", m_algorithm);
    savegame.writeEntry("Moves", QStringList());
}

void KpegMainWindow::updateMoves(int count)
{
    statusBar()->changeItem(i18n("Moves: %1", count) , 1);
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
    statusBar()->changeItem(i18n("Time: %1", timeStr), 2);
}

void KpegMainWindow::levelChanged(KGameDifficulty::standardLevel)
{
    KGameDifficulty::setLevel(KGameDifficulty::level());
    KpegSettings::self()->writeConfig();

    loadGame();
}

void KpegMainWindow::showHighscores()
{
    KScoreDialog ksdialog(KScoreDialog::Name | KScoreDialog::Level | KScoreDialog::Time, this);
    ksdialog.setConfigGroup(KGameDifficulty::localizedLevelString());
    ksdialog.exec();
}
