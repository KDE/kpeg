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

#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsView>
#include <QHash>
#include <QSvgRenderer>
#include <QUndoStack>

#include <KGameRenderer>

class KGamePopupItem;
class Hole;
class Peg;

/**
 * @brief The game board.
 *
 * This class controls all of the game objects, as well as setting the theme.
 */
class Board : public QGraphicsView
{
    Q_OBJECT
public:
    /**
    * Constructs a new game board.
    *
    * @param moves Where to store the history of peg movements.
    * @param parent The game board's parent widget.
    */
    explicit Board(QWidget* parent = 0);

    /**
    * Returns true if there is only one peg left; otherwise returns false.
    */
    bool isFinished() const {
        return m_status;
    }

    /**
    * Returns true if @p hole is an empty hole.
    *
    * @param hole The hole to check.
    */
    bool isHole(const QPoint& hole) const;

    /**
    * Returns true if @p hole has a peg.
    *
    * @param hole The hole to check.
    */
    bool isPeg(const QPoint& hole) const;

    /**
    * Returns the hole specified by @p hole.
    *
    * @param hole The hole to fetch.
    */
    Hole* hole(const QPoint& hole) const;

    /**
    * Creates a new puzzle layout.
    *
    * @param seed The seed passed to the random number generator.
    * @param difficulty How hard of a layout to create.
    */
    void generate(int difficulty, int algorithm);

    /**
    * Moves a peg from @p ole_hole to @p new_hole.
    *
    * @param old_hole The source hole.
    * @param new_hole The destination hole.
    */
    void move(const QPoint& old_hole, const QPoint& new_hole);

    /**
    * Changes the appearance of the board.
    *
    * @param theme The theme to use.
    */
    void setTheme();
    void setGamePaused(bool paused);
    
    KGameRenderer* renderer() { return &m_renderer; }

signals:
    void countChanged(int);

protected:
    virtual void drawBackground(QPainter* painter, const QRectF& rect);
    virtual void resizeEvent(QResizeEvent* event);

private:
    bool checkFinished();
    void showMessage();

private:
    QHash<QPoint, Hole*> m_holes;
    int m_status;
    KGamePopupItem* m_message;
    KGameRenderer m_renderer;
    QUndoStack* m_moves;
    QSvgRenderer* m_theme;
    Peg* m_peg;
};

#endif // BOARD_H
