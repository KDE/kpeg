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

#ifndef PEGE_HOLE_H
#define PEGE_HOLE_H

#include <QGraphicsSvgItem>
class Peg;

/**
* @brief A hole that can contain a peg.
*/
class Hole : public QGraphicsSvgItem
{
public:
    /**
    * Constructs a hole.
    *
    * @param position The location of the hole.
    * @param parent The parent item of the hole.
    */
    explicit Hole(const QPoint& position, QGraphicsItem* parent = 0);

    // Returns true if the hole has a peg.
    bool hasPeg() const {
        return m_peg != 0;
    }

    // Returns the peg contained by the hole.
    Peg* peg() const {
        return m_peg;
    }

    // Sets the peg contained by the hole.
    void setPeg(Peg* peg) {
        m_peg = peg;
    }

    // Sets the highlighted status of the hole.
    void setHighlighted(bool highlight);

private:
    Peg* m_peg;
};

#endif // PEGE_HOLE_H
