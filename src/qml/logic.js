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

function startNewGame(boardStyle) {
    selectedBoardStyle = boardStyle
    board = new Array(maxRow);

    for (var i = 0; i < maxColumn; i++) {
        board[i] = new Array(maxColumn)
    }

    for (var row = 0; row < maxRow; row++) {
        for (var column = 0; column < maxColumn; column++) {
            board[row][column] = null;

            if (row == Math.floor(maxRow / 2) && column == Math.floor(maxColumn / 2)) {
                createPegOrHole(row, column, "hole");
	    } else {
                createPegOrHole(row, column, "peg");
	    }

            switch (selectedBoardStyle) {
	        default:
                case "English":
                    if (isEnglishBoardCoord(column, row)) {
                        board[row][column].visible = true;
                    } else {
                        board[row][column].visible = false;
                    }
                    break;
	        case "European":
                    if (isEuroperanBoardCoord(column, row)) {
                        board[row][column].visible = true;
                    } else {
                        board[row][column].visible = false;
                    }
                    break;
            }
        }
    }
}

var pegComponent
function createPegOrHole(row, column, pegOrHole) {

    if (pegComponent == null)
        pegComponent = Qt.createComponent("Peg.qml");

    if (pegComponent.status == Component.Ready) {
        var dynamicObject;

        if (pegOrHole == "hole") {
            dynamicObject = pegComponent.createObject(gameCanvas, { "currentState": "HOLE" });
	} else if (pegOrHole == "peg") {
            dynamicObject = pegComponent.createObject(gameCanvas, { "currentState": "PEG" });
	}

        if (dynamicObject == null) {
            console.log(peg.errorString());
            return false;
        }

        dynamicObject.x = column * gameCanvas.pegSize;
        dynamicObject.y = row * gameCanvas.pegSize;
        dynamicObject.width = gameCanvas.pegSize;
        dynamicObject.height = gameCanvas.pegSize;
        
        // console.log("(" + row + "," + column + ")" + "x:"  + dynamicObject.x + " y:" + dynamicObject.y)
        board[row][column] = dynamicObject;
    } else {
        console.log(pegComponent.errorString());
        return false;
    }
}

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

function destroyBoard() {
    for (var i = 0; i < maxColumn; i++) {
        for (var j = 0 ; j < maxRow; j++) {
            if (board[i][j] != null) {
                board[i][j].destroy();
            }
        }
    }
    delete board;
}

function capture(hightlightPeg) {
    var capturePeg = getCapturePeg(lastSelectedPeg, hightlightPeg);
    if (capturePeg != null) {
        capturePeg.currentState = "HOLE";
        lastSelectedPeg.currentState = "HOLE";
        hightlightPeg.currentState = "PEG";
        lastSelectedPeg = null;
    }
}

function getCapturePeg(pegBefore, pegAfter) {
    var beforeX;
    var beforeY;
    var afterX;
    var afterY;

    if (pegBefore.currentState == "SELECTED") {
        if (pegAfter.currentState == "HIGHLIGHT") {
            beforeX = pegBefore.x / gameCanvas.pegSize;
            afterX = pegAfter.x / gameCanvas.pegSize;
            beforeY = pegBefore.y / gameCanvas.pegSize;
            afterY = pegAfter.y / gameCanvas.pegSize;

            var targetDistance = 2;
            if ((Math.abs(beforeX - afterX) != targetDistance) && (Math.abs(beforeY - afterY) != targetDistance))
                return;

            if (afterX != beforeX && afterY != beforeY)
                return

            var capturePegX;
            var capturePegY;

            if (afterX > beforeX) {
                capturePegX = beforeX + 1;
	    } else if (afterX < beforeX) {
                capturePegX = afterX + 1;
	    } else {
                capturePegX = afterX;
	    }

            if (afterY > beforeY) {
                capturePegY = beforeY + 1;
	    } else if (afterY < beforeY) {
                capturePegY = afterY + 1;
	    } else {
                capturePegY = afterY;
	    }

            var capturePeg = board[capturePegY][capturePegX];
            if (capturePeg.currentState == "HOLE")
                return;

            // console.log("Captured peg: (" + capturePegX + ", " + capturePegY + ")");
            return capturePeg;
        }
    }
}
