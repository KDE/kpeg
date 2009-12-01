/*
  Copyright 2009  Graeme Gott <graeme@gottcode.org>

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

#ifndef PEGE_MOVEMENT_H
#define PEGE_MOVEMENT_H

#include <QHash>
#include <QPoint>
#include <QUndoCommand>

class Board;
class Hole;
class Peg;

/**
 * @brief A movement of a peg.
 */
class Movement : public QUndoCommand
{
public:
    /**
     * Constructs a movement.
     *
     * @param start_hole The hole the peg jumped from.
     * @param end_hole The hole the peg jumped to.
     * @param board The board that contains the holes.
    */
    Movement(const QPoint& start_hole, const QPoint& end_hole, Board* board);

    // Move the peg from the start hole to the end hole, removing the peg in the hole between them.
    virtual void redo();

    // Move the peg from the end hole to the start hole, restoring the peg in the hole between them.
    virtual void undo();

private:
    Peg* m_peg;
    Peg* m_jumped_peg;
    QPoint m_start_hole;
    QPoint m_jumped_hole;
    QPoint m_end_hole;
    Board* m_board;
};

#endif // PEGE_MOVEMENT_H
