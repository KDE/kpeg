/*
  Copyright 2009  Graeme Gott <graeme@gottcode.org>
  Copyright 2010  Ronny Yabar <ronnycontacto@gmail.com>

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

#include "puzzle.h"

#include <algorithm>
#include <cstdlib>

inline uint qHash(const QPoint& key)
{
    return (key.x() << 16) + key.y();
}

Puzzle::Puzzle()
{
    m_directions = QList<QPoint>()
                   << QPoint(-1, 0)
                   << QPoint(-1, -1)
                   << QPoint(0, -1)
                   << QPoint(1, -1)
                   << QPoint(1, 0)
                   << QPoint(1, 1)
                   << QPoint(0, 1)
                   << QPoint(-1, 1);
}

Puzzle::~Puzzle()
{
}

QHash<QPoint, bool> Puzzle::holes() const
{
    return m_holes;
}

QPoint Puzzle::position() const
{
    return m_top_left;
}

QSize Puzzle::size() const
{
    return QSize((m_bottom_right.x() - m_top_left.x()) + 1, (m_bottom_right.y() - m_top_left.y()) + 1);
}

void Puzzle::generate(int difficulty)
{
    difficulty += 5;
    generate_pegs(difficulty);
}

void Puzzle::generate_pegs(int pegs)
{
    QPoint start(0, 0);
    setHasPeg(start, true);
    findMoves(start, pegs);
}

bool Puzzle::isAvailable(const QPoint& hole) const
{
    return hasPeg(hole) != 1;
}

void Puzzle::shuffle(QList<QPoint>& pegs) const
{
    std::random_shuffle(pegs.begin(), pegs.end());
}

bool Puzzle::findNextMove(const QPoint& start_hole, QPoint& jumped_hole, QPoint& end_hole)
{
    std::random_shuffle(m_directions.begin(), m_directions.end());
    foreach(const QPoint& direction, m_directions) {
        jumped_hole = direction + start_hole;
        end_hole = (direction * 2) + start_hole;
        if (isAvailable(jumped_hole) && isAvailable(end_hole)) {
            setHasPeg(start_hole, false);
            setHasPeg(jumped_hole, true);
            setHasPeg(end_hole, true);
            return true;
        }
    }
    return false;
}

QPoint Puzzle::findMoves(const QPoint& start, int loops)
{
    QList<QPoint> pegs;
    pegs.append(start);

    QPoint jumped_hole, end_hole;
    for (int i = 0; i < loops; ++i) {
        shuffle(pegs);
        foreach(const QPoint& start_hole, pegs) {
            if (findNextMove(start_hole, jumped_hole, end_hole)) {
                pegs.removeOne(start_hole);
                pegs.append(jumped_hole);
                pegs.append(end_hole);
                break;
            }
        }
    }

    return pegs.last();
}

int Puzzle::hasPeg(const QPoint& hole) const
{
    return m_holes.contains(hole) ? m_holes.value(hole) : -1;
}

void Puzzle::setHasPeg(const QPoint& hole, bool value)
{
    m_top_left.setX(qMin(m_top_left.x(), hole.x()));
    m_top_left.setY(qMin(m_top_left.y(), hole.y()));
    m_bottom_right.setX(qMax(m_bottom_right.x(), hole.x()));
    m_bottom_right.setY(qMax(m_bottom_right.y(), hole.y()));
    m_holes[hole] = value;
}

void PuzzleBranch::generate_pegs(int pegs)
{
    QPoint start_hole(0, 0);
    setHasPeg(start_hole, true);

    int loops = pegs;
    int base = loops / 2;
    
    while (loops > 0) {
        int group_loops = qMin(base,loops);
        start_hole = findMoves(start_hole, group_loops);
        loops -= group_loops;

        QPoint jumped_hole, end_hole;
        int branch_loops = qMin(base,loops);
        
	for (int i = 0; i < branch_loops; ++i) {
            if (findNextMove(start_hole, jumped_hole, end_hole)) {
                start_hole = end_hole;
                loops--;
            }
        }
    }
}

bool PuzzleLine::isAvailable(const QPoint& hole) const
{
    return hasPeg(hole) == -1;
}

void PuzzleLine::shuffle(QList<QPoint>&) const
{
}
