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

var lastSelectedPeg;
var hightlight_peg_left;
var hightlight_peg_right;
var hightlight_peg_up;
var hightlight_peg_down;

function clickPeg(mouseX, mouseY) {
    var column = Math.floor(mouseX / gameCanvas.pegSize);
    var row = Math.floor(mouseY / gameCanvas.pegSize);
    var peg = board[row][column];

    console.log("Selected Peg: (" + row + ", " + column + ")");
    if (column >= maxColumn || column < 0 || row >= maxRow || row < 0)
        return;
    if (board[column][row] == null)
        return;

    var interval = 2
    var peg_left = column - interval
    var peg_right = column + interval
    var peg_up = row - interval
    var peg_down = row + interval
    
    if (lastSelectedPeg == null) {
        if (peg.currentState == "PEG") {
            lastSelectedPeg = peg;
            lastSelectedPeg.currentState = "SELECTED";
        }
        //return;
    }

    if (peg.currentState == "PEG") {
        console.log("Is peg")
        lastSelectedPeg.currentState = "PEG";
        lastSelectedPeg = peg;
        lastSelectedPeg.currentState = "SELECTED";
	
	if (hightlight_peg_left != null) {
	  if (hightlight_peg_left.currentState == "HIGHLIGHT") hightlight_peg_left.currentState = "HOLE";
	}
	if (hightlight_peg_right != null) {
	  if (hightlight_peg_right.currentState == "HIGHLIGHT") hightlight_peg_right.currentState = "HOLE"; 
	}
	if (hightlight_peg_up != null) {
	  if (hightlight_peg_up.currentState == "HIGHLIGHT") hightlight_peg_up.currentState = "HOLE"; 
	}
	if (hightlight_peg_down != null) {
	  if (hightlight_peg_down.currentState == "HIGHLIGHT") hightlight_peg_down.currentState = "HOLE"; 
	}
    }
    
    if (peg.currentState == "SELECTED") {
        console.log("Is selected")
	if (peg_left >= 0 && peg_left < maxColumn) {
	    var captured_peg_left = board[row][column - 1]
	    hightlight_peg_left = board[row][column - 2]
	    if (peg.currentState == "SELECTED" && 
                captured_peg_left.currentState == "PEG" && 
                hightlight_peg_left.currentState == "HOLE") { 
		hightlight_peg_left.currentState = "HIGHLIGHT";
	    }
	}
	
	if (peg_right >= 0 && peg_right < maxColumn) {
	    var captured_peg_right = board[row][column + 1]
	    hightlight_peg_right = board[row][column + 2]
	    if (peg.currentState == "SELECTED" && 
                captured_peg_right.currentState == "PEG" && 
                hightlight_peg_right.currentState == "HOLE") { 
		hightlight_peg_right.currentState = "HIGHLIGHT";
	    }
	}
	
	if (peg_up >= 0 && peg_up < maxRow) {
	  var captured_peg_up = board[row - 1][column]
	  hightlight_peg_up = board[row - 2][column]
	  if (peg.currentState == "SELECTED" && 
              captured_peg_up.currentState== "PEG" && 
              hightlight_peg_up.currentState == "HOLE") { 
	      hightlight_peg_up.currentState = "HIGHLIGHT";
	  }
	}
	
	if (peg_down >= 0 && peg_down < maxRow) {
	  var captured_peg_down = board[row + 1][column]
	  hightlight_peg_down = board[row + 2][column]
	  if (peg.currentState == "SELECTED" && 
              captured_peg_down.currentState == "PEG" && 
              hightlight_peg_down.currentState == "HOLE") {
	      hightlight_peg_down.currentState = "HIGHLIGHT";
	  }
	}
    }

    if (peg.currentState == "HIGHLIGHT") {
        console.log("Is highlighted")
        capture(peg);
        if (hightlight_peg_left != null) {
	  if (hightlight_peg_left.currentState == "HIGHLIGHT") hightlight_peg_left.currentState = "HOLE"; 
	}
	if (hightlight_peg_right != null) {
	  if (hightlight_peg_right.currentState == "HIGHLIGHT") hightlight_peg_right.currentState = "HOLE"; 
	}
	if (hightlight_peg_up != null) {
	  if (hightlight_peg_up.currentState == "HIGHLIGHT") hightlight_peg_up.currentState = "HOLE"; 
	}
	if (hightlight_peg_down != null) {
	  if (hightlight_peg_down.currentState == "HIGHLIGHT") hightlight_peg_down.currentState = "HOLE"; 
	}
    }
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

function checkWin() {
    var pegCounter = 0;
    for (var row = 0; row < maxRow; row++) {
        for (var column = 0; column < maxColumn; column++) {
            var peg = board[row][column];
            if (peg.currentState == "PEG" && peg.visible == true ) {
                pegCounter++;
            }
        }
    }
    
    if (pegsToWin == pegCounter) {
        timer.enabled = false;
        view.state = "WINNER_DIALOG";
    }
}
