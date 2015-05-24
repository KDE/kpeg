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

#include "board.h"
#include "hole.h"
#include "movement.h"
#include "peg.h"
#include "puzzle.h"
#include "settings.h"

#include <QUndoStack>
#include <KConfigGroup>
#include <KGamePopupItem>
#include <KgTheme>
#include <KgThemeProvider>
#include <KLocalizedString>
#include <KSharedConfig>

inline uint qHash(const QPoint& key)
{
    return (key.x() << 16) + key.y();
}

static KgThemeProvider* provider()
{
    KgThemeProvider* prov = new KgThemeProvider;
    prov->discoverThemes("appdata", QLatin1String("themes"), QLatin1String("default"));
    return prov;
}

Board::Board(QWidget* parent)
        : QGraphicsView(parent),
        m_status(0),
        m_movesCount(0),
        m_renderer(provider())
{
    m_moves = new QUndoStack();
    QGraphicsScene* scene = new QGraphicsScene(this);
    setScene(scene);
    // Load theme
    m_theme = new QSvgRenderer(this);  
    setTheme();

    m_soundMove = new KgSound(QStandardPaths::locate(QStandardPaths::DataLocation,
                                                    QLatin1Literal("sounds/move.wav")), this);

    // Configure view
    setCacheMode(QGraphicsView::CacheBackground);
    setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    setFrameStyle(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMinimumSize(400, 400);
}

bool Board::isHole(const QPoint& hole) const
{
    return m_holes.contains(hole) ? !m_holes.value(hole)->hasPeg() : 0;
}

bool Board::isPeg(const QPoint& hole) const
{
    return m_holes.contains(hole) ? m_holes.value(hole)->hasPeg() : 0;
}

Hole* Board::hole(const QPoint& hole) const
{
    return m_holes.contains(hole) ? m_holes[hole] : 0;
}

void Board::generate(int difficulty, int algorithm)
{
    // Remove old puzzle
    m_status = 0;
    m_movesCount = 0;
    m_message = 0;
    m_moves->clear();
    m_holes.clear();
    
    scene()->clear();
    setInteractive(true);

    // Create puzzle
    Puzzle* puzzle = 0;
    
    switch (algorithm) {
    case 1:
    default:
        puzzle = new Puzzle;
        break;
    case 2:
        puzzle = new PuzzleBranch;
        break;
    case 3:
        puzzle = new PuzzleLine;
        break; 
    }
    
    puzzle->generate(difficulty);

    // Create scene
    setSceneRect(QRectF(puzzle->position() * 22, puzzle->size() * 22).adjusted(-10, -10, 10, 10));
    fitInView(sceneRect(), Qt::KeepAspectRatio);

    QHash<QPoint, bool> holes = puzzle->holes();
    QHashIterator<QPoint, bool> i(holes);

    while (i.hasNext()) {
        i.next();
        QPoint position = i.key();

        Hole* hole = new Hole(position);
        hole->setSharedRenderer(m_theme);
        scene()->addItem(hole);
        m_holes.insert(position, hole);

        if (i.value()) {
            Peg* peg = new Peg(position, this);
            peg->setSharedRenderer(m_theme);
            hole->setPeg(peg);
            scene()->addItem(peg);
        }
    }

    // Add message
    m_message = new KGamePopupItem;
    m_message->setMessageOpacity(0.9);
    m_message->setMessageTimeout(0);
    scene()->addItem(m_message);

    delete puzzle;
}

void Board::move(const QPoint& old_hole, const QPoint& new_hole)
{
    if (KpegSettings::playSounds()) {
        m_soundMove->start();
    }

    // Move peg
    Movement* movement = new Movement(old_hole, new_hole, this);
    m_moves->push(movement);

    m_movesCount++;
    emit countChanged(m_movesCount);

    // Handle finishing the game
    if (checkFinished()) {
        setInteractive(false);
        showMessage();
        m_moves->clear();

        // Remove saved game
        KConfig *config = KSharedConfig::openConfig().data();
        KConfigGroup savegame = config->group("Game");
        savegame.writeEntry("Moves", QStringList());
    }
}

void Board::setTheme()
{ 
    const KgTheme* theme = m_renderer.theme();
    m_theme->load(theme->graphicsPath());

    foreach(QGraphicsItem* item, scene()->items()) {
        QGraphicsSvgItem* svg = qgraphicsitem_cast<QGraphicsSvgItem*>(item);
        if (svg) {
            svg->setCacheMode(QGraphicsItem::NoCache);
            svg->setSharedRenderer(m_theme);
            svg->update();
            svg->setCacheMode(QGraphicsItem::DeviceCoordinateCache);	    
        }
    }
    scene()->invalidate();
}

void Board::setGamePaused(bool paused)
{
    if (paused) {
        m_message->showMessage(i18n("Game is paused."), KGamePopupItem::Center);
    } else {
        m_message->forceHide();
    }
}

void Board::drawBackground(QPainter* painter, const QRectF& rect)
{
    if (m_theme->isValid()) {
        m_theme->render(painter, QLatin1String("background"), rect);
    }
}

void Board::resizeEvent(QResizeEvent* event)
{
    fitInView(sceneRect(), Qt::KeepAspectRatio);
    showMessage();
    QGraphicsView::resizeEvent(event);
}

bool Board::checkFinished()
{
    int pegs = 0;
    foreach(Hole* hole, m_holes) {
        if (hole->hasPeg()) {
            pegs++;
            if (hole->peg()->canMove()) {
                return false;
            }
        }
    }
    m_status = (pegs == 1) ? 2 : 1;
    return true;
}

void Board::showMessage()
{
    switch (m_status) {
    case 1:
        m_message->showMessage(i18n("You have lost."), KGamePopupItem::TopLeft);
        break;
    case 2:
        m_message->showMessage(i18n("Congratulations, you have won!"), KGamePopupItem::TopLeft);
        break;
    default:
        break;
    }
}
