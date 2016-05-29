/*
  Copyright 2009  Graeme Gott <graeme@gottcode.org>
  Copyright 2010  Ronny Yabar Aizcorbe <ronny@kde.org>

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
#include "gameview.h"
#include "settings.h"

#include <QQuickItem>

#include <KActionCollection>
#include <KStandardGameAction>
#include <KToggleAction>
#include <KConfigDialog>
#include <KConfigGroup>
#include <KScoreDialog>
#include <KgSound>
#include <KgThemeSelector>
#include <KLocalizedString>
#include <KMessageBox>
#include <KSharedConfig>

KpegMainWindow::KpegMainWindow(QWidget* parent)
        : KXmlGuiWindow(parent), m_view(new GameView(this))
{
    connect(this, SIGNAL(load(QVariant)), m_view->rootObject(), SLOT(loadGame(QVariant)));
    connect(this, SIGNAL(pause(QVariant)), m_view->rootObject(), SLOT(pause(QVariant)));
    connect(this, SIGNAL(start()), m_view->rootObject(), SLOT(startGame()));

    setCentralWidget(m_view);
    setupActions();
    setupGUI();

    loadGame();
}

KpegMainWindow::~KpegMainWindow()
{
}

void KpegMainWindow::setupActions()
{
    KStandardGameAction::gameNew(this, SLOT(newGame()), actionCollection());
    m_actionPause = KStandardGameAction::pause(this, SLOT(pauseGame(bool)), actionCollection());
    KStandardGameAction::highscores(this, SLOT(showHighscores()), actionCollection());
    KStandardGameAction::quit(this, SLOT(close()), actionCollection());
    KStandardAction::preferences(this, SLOT(configureSettings()), actionCollection());

    m_playSoundsAction = new KToggleAction(i18n("&Play Sounds"), this);
    actionCollection()->addAction(QLatin1String("play_sounds"), m_playSoundsAction);
    m_playSoundsAction->setChecked(KpegSettings::playSounds());
    connect(m_playSoundsAction, SIGNAL(triggered(bool)), this, SLOT(setSounds(bool)));
}

void KpegMainWindow::configureSettings()
{
    if (KConfigDialog::showDialog(QLatin1String("settings"))) {
        return;
    }
    KConfigDialog* dialog = new KConfigDialog(this, QLatin1String("settings"), KpegSettings::self());
    dialog->addPage(new KgThemeSelector(m_view->getProvider()), i18n("Theme"), QLatin1String("games-config-theme"));
    connect( dialog, SIGNAL(settingsChanged(const QString&)), this, SLOT(loadSettings()) );
    dialog->show();
}

void KpegMainWindow::loadSettings()
{
    KConfig *config = KSharedConfig::openConfig().data();
    KConfigGroup savegame = config->group("Game");

}

void KpegMainWindow::loadGame()
{
    KConfig *config = KSharedConfig::openConfig().data();
    KConfigGroup savegame = config->group("Game");
    load(savegame.readEntry("Board"));

    m_actionPause->setEnabled(true);
}

void KpegMainWindow::newGame()
{
    start();
    m_actionPause->setEnabled(true);
    if (m_actionPause->isChecked()) {
        m_view->setGamePaused(false);
        m_actionPause->setChecked(false);
    }
}

void KpegMainWindow::pauseGame(bool paused)
{
    pause(paused);
}


void KpegMainWindow::setSounds(bool enable)
{
    KpegSettings::setPlaySounds(enable);
    KpegSettings::self()->save();
}

void KpegMainWindow::showHighscores()
{
    KScoreDialog ksdialog(KScoreDialog::Name | KScoreDialog::Level | KScoreDialog::Time, this);
    /*ksdialog.setConfigGroup(qMakePair(
      //  Kg::difficulty()->currentLevel()->key(),
        //Kg::difficulty()->currentLevel()->title()
    ));*/
    ksdialog.exec();
}
