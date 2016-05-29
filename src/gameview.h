/*
    Copyright 2016  Ronny Yabar Aizcorbe <ronny@kde.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <KgDeclarativeView>
#include <KgThemeProvider>

class GameView : public KgDeclarativeView
{
    Q_OBJECT
public:
    explicit GameView(QWidget *parent=0);
    virtual ~GameView();

    KgThemeProvider* getProvider() { return m_provider; }
    void setGamePaused(bool paused);

private:
    KgThemeProvider* m_provider;

};

#endif //GAMEVIEW_H
