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

#include "movement.h"

#include "board.h"
#include "hole.h"
#include "peg.h"

/*****************************************************************************/

Movement::Movement(const QPoint& start_hole, const QPoint& end_hole, Board* board)
: m_start_hole(start_hole),
  m_jumped_hole(start_hole + ((end_hole - start_hole) / 2)),
  m_end_hole(end_hole),
  m_board(board) {
	Q_ASSERT(m_board->isPeg(m_start_hole));
	Q_ASSERT(m_board->isPeg(m_jumped_hole));
	Q_ASSERT(m_board->isHole(m_end_hole));

	m_peg = m_board->hole(start_hole)->peg();
	m_jumped_peg = m_board->hole(m_jumped_hole)->peg();

	setText(QString("%1x%2:%3x%4").arg(m_start_hole.x()).arg(m_start_hole.y()).arg(m_end_hole.x()).arg(m_end_hole.y()));
}

/*****************************************************************************/

void Movement::redo() {
	m_board->hole(m_start_hole)->setPeg(0);
	m_board->hole(m_jumped_hole)->setPeg(0);
	m_board->hole(m_end_hole)->setPeg(m_peg);

	m_jumped_peg->hide();
	m_peg->move(m_end_hole);
}

/*****************************************************************************/

void Movement::undo() {
	m_board->hole(m_start_hole)->setPeg(m_peg);
	m_board->hole(m_jumped_hole)->setPeg(m_jumped_peg);
	m_board->hole(m_end_hole)->setPeg(0);

	m_peg->move(m_start_hole);
	m_jumped_peg->show();
}

/*****************************************************************************/
