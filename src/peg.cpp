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

#include "peg.h"

#include "board.h"
#include "hole.h"

/*****************************************************************************/

Peg::Peg(const QPoint& hole, Board* board, QGraphicsItem* parent)
: QGraphicsSvgItem(parent),
  m_hole(hole),
  m_board(board) {
	setElementId("peg");
	setZValue(2);
	setPos(m_hole.x() * 20, m_hole.y() * 20);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setCursor(Qt::OpenHandCursor);
}

/*****************************************************************************/

bool Peg::canMove() {
	findHoles();
	return !m_holes.isEmpty();
}

/*****************************************************************************/

void Peg::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	findHoles();
	showAvailableHoles(true);

	setZValue(3);
	setCursor(Qt::ClosedHandCursor);
	QGraphicsSvgItem::mousePressEvent(event);
}

/*****************************************************************************/

void Peg::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
	showAvailableHoles(false);

	QPoint hole = ((mapToScene(boundingRect().center()) / 20.f) - QPointF(0.5f, 0.5f)).toPoint();
	if (m_holes.contains(hole)) {
		m_board->move(m_hole, hole);
	}
	move(m_hole);

	setZValue(2);
	if (!m_board->isFinished()) {
		setCursor(Qt::OpenHandCursor);
	} else {
		unsetCursor();
	}
	QGraphicsSvgItem::mouseReleaseEvent(event);
}

/*****************************************************************************/

void Peg::move(QPoint hole) {
	m_hole = hole;
	setPos(m_hole.x() * 20, m_hole.y() * 20);
}

/*****************************************************************************/

void Peg::findHoles() {
	m_holes.clear();
	for (int r = -1; r < 2; ++r) {
		for (int c = -1; c < 2; ++c) {
			QPoint direction(c, r);
			QPoint hole = m_hole + (direction * 2);
			if (m_board->isHole(hole) && m_board->isPeg(m_hole + direction)) {
				m_holes.append(hole);
			}
		}
	}
}

/*****************************************************************************/

void Peg::showAvailableHoles(bool show) {
	foreach (const QPoint& hole, m_holes) {
		m_board->hole(hole)->setHighlighted(show);
	}
}

/*****************************************************************************/
