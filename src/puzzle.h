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

#ifndef PEGE_PUZZLE_H
#define PEGE_PUZZLE_H

#include <QHash>
#include <QPoint>
#include <QSize>

/**
 * @brief Contains the layout of where the holes are, and which have pegs.
 */
class Puzzle
{
public:
    Puzzle();
    virtual ~Puzzle();

    // Returns the layout as a hash of points describing the holes.
    QHash<QPoint, bool> holes() const;

    // Returns the top left corner of the layout.
    QPoint position() const;

    // Returns the size of the layout.
    QSize size() const;

    /**
    * Creates a layout.
    *
    * @param seed The seed passed to the random number generator.
    * @param difficulty How hard of a layout to create.
    */
    void generate(int seed, int difficulty);

protected:
    /**
    * Creates a layout with a single group of pegs.
    *
    * @param pegs How many pegs to create.
    */
    virtual void generate(int pegs);

    /**
    * Returns true if @p hole has no peg.
    *
    * @param hole The hole to check.
    */
    virtual bool isAvailable(const QPoint& hole) const;

    /**
    * Randomly shuffles a list of pegs
    *
    * @param pegs The pegs to shuffle.
    */
    virtual void shuffle(QList<QPoint>& pegs) const;

    /**
    * Determines where to move next.
    *
    * @param start_hole The hole that is moved from.
    * @param jumped_hole The hole between the start and end holes.
    * @param end_hole The hole that is moved to.
    */
    bool findNextMove(const QPoint& start_hole, QPoint& jumped_hole, QPoint& end_hole);

    /**
    * Creates a group of pegs.
    *
    * @param start The initial peg.
    * @param loops How many pegs to create.
    */
    QPoint findMoves(const QPoint& start, int loops);

    /**
    * Returns whether or not the @p hole has a peg.
    *
    * @param hole The hole to check.
    * @return Returns 0 if the hole is empty, 1 if it has a peg, and -1 if it doesn't exist.
    */
    int hasPeg(const QPoint& hole) const;

    /**
    * Sets if the @p hole has a peg.
    *
    * @param hole The hole to specify.
    * @param value What to put at the hole.
    */
    void setHasPeg(const QPoint& hole, bool value);

private:
    QHash<QPoint, bool> m_holes;
    QList<QPoint> m_directions;
    QPoint m_top_left;
    QPoint m_bottom_right;
};

/**
 * @brief A Puzzle that has branches between groups of pegs.
 */
class PuzzleBranch : public Puzzle
{
protected:
    /**
    * Creates a layout with branches between the groups.
    *
    * @param pegs How many pegs to create.
    */
    virtual void generate(int pegs);
};

/**
 * @brief A Puzzle that doesn't reuse or shuffle any holes.
 */
class PuzzleLine : public Puzzle
{
protected:
    /**
    * Returns true if @p hole does not exist yet.
    *
    * @param hole The hole to check.
    */
    virtual bool isAvailable(const QPoint& hole) const;

    /**
    * An empty function to prevent shuffling the pegs.
    *
    * @param pegs The pegs to shuffle.
    */
    virtual void shuffle(QList<QPoint>& pegs) const;
};

#endif // PEGE_PUZZLE_H
