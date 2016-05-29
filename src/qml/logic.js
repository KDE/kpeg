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

var maxColumn = 7;
var maxRow = 7;
var pegsToWin = 1;
var board;
var selectedBoardStyle;

function isEnglishBoardCoord(column, row) {
    /* English board
      0 1 2 3 4 5 6
    0     o o o
    1     o o o
    2 o o o o o o o
    3 o o o x o o o
    4 o o o o o o o
    5     o o o
    6     o o o */

    if (row < 2 || row > 4) {
        if (column  >= 2 && column <= 4)
            return true;
    }
    if (row >= 2 && row <= 4 ) {
        return true;
    }

    return false;
}

function isEuroperanBoardCoord(column, row) {
    /* European board
      0 1 2 3 4 5 6
    0     o o o
    1   o o o o o
    2 o o o o o o o
    3 o o o x o o o
    4 o o o o o o o
    5   o o o o o
    6     o o o */

    if (row < 2 || row > 4) {
        if (column  >= 2 && column <= 4)
            return true;
    }

    if ((row == 1 && column == 1) ||
        (row == 5 && column == 5) ||
        (row == 1 && column == 5) ||
        (row == 5 && column == 1))
        return true;

    if (row >= 2 && row <= 4 ) {
        return true;
    }

    return false;
}
