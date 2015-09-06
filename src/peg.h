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

#ifndef PEG_H
#define PEG_H

#include <QGraphicsSvgItem>
#include <KgSound>

class Board;

/**
 * @brief A peg movable by the player.
 */
class Peg : public QGraphicsSvgItem
{
    Q_OBJECT
public:
    /**
     * Constructs a peg.
     *
     * @param hole The hole containing the peg.
     * @param board The board containing the peg.
     * @param parent The parent item of the peg.
     */
    Peg();
    Peg(const QPoint& hole, Board* board, QGraphicsItem* parent = 0);

    /**
     * Returns true if the peg can move.
     */
    bool canMove();

    /**
     * Moves the peg.
     *
     * @param hole Where to move the peg.
     */
    void move(QPoint hole);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

private:
    void findHoles();
    void showAvailableHoles(bool show);


private:
    QPoint m_hole;
    QList<QPoint> m_holes;
    Board* m_board;
    KgSound *m_soundPeg;

};

#endif // PEG_H
