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

#include "gameview.h"
#include "settings.h"

#include <QGraphicsObject>
#include <QQuickItem>

#include <KgThemeProvider>


GameView::GameView(QWidget *parent) :
    KgDeclarativeView(parent),
    m_provider(new KgThemeProvider)
{
    m_provider->discoverThemes("appdata", QLatin1String("themes"));
    m_provider->setDeclarativeEngine(QLatin1String("themeProvider"), engine());
    
    QString path = QStandardPaths::locate(QStandardPaths::AppDataLocation, 
                                          QLatin1String("qml/main.qml"));
    setSource(QUrl::fromLocalFile(path));
}

GameView::~GameView()
{
    delete m_provider;
}

void GameView::setGamePaused(bool paused)
{
    QMetaObject::invokeMethod(rootObject(), "pause",
                              Q_ARG(QVariant, paused));
}
