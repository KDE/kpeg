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

#ifndef WINDOW_H
#define WINDOW_H

#include <KXmlGuiWindow>
#include <KgDifficulty>

class KUndoStack;
class Board;
class KGameClock;
class KToggleAction;


class KpegMainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:

    KpegMainWindow(QWidget* parent = 0);
    ~KpegMainWindow();

private slots:
    void newGame();
    void restartGame();
    void configureSettings();
    void loadSettings();
    void updateTimer(const QString&);
    void pauseGame(bool paused);
    void updateMoves(int count);
    void levelChanged();
    void showHighscores();

private:
    void loadGame();
    void startGame(int);
    void setupActions();

    Board* m_board;

    KUndoStack* m_moves;
    KGameClock* m_gameClock;
    KToggleAction* m_actionPause;

    int m_difficulty;
    int m_algorithm;

};

#endif // WINDOW_H
