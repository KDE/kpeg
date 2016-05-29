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

#ifndef WINDOW_H
#define WINDOW_H

#include <KXmlGuiWindow>

class KToggleAction;
class GameView;


class KpegMainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:

    explicit KpegMainWindow(QWidget* parent = 0);
    ~KpegMainWindow();

signals:
    void load(QVariant boardStyle);
    void pause(QVariant paused);
    void start();

private slots:
    void configureSettings();
    void loadSettings();
    void loadGame();
    void newGame();
    void pauseGame(bool paused);
    void setSounds(bool enable);
    void showHighscores();

private:
    void setupActions();

    GameView *m_view;
    KToggleAction* m_actionPause;
    KToggleAction* m_playSoundsAction;
};

#endif // WINDOW_H
