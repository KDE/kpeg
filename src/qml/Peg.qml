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

import QtQuick 2.3

Rectangle {
    id: root

    property string currentState
    
    radius: width * 0.5
    color: "transparent"

    state: currentState
    states: [
        State {
            name: "HOLE"
            PropertyChanges {
                target: peg; spriteKey: "hole"
            }
        },
        State {
            name: "PEG"
            PropertyChanges {
                target: peg; spriteKey: "peg";
            }
        },
	State {
            name: "HIGHLIGHT"
            PropertyChanges {
                target: peg; spriteKey: "highlight";
            }
        },
        State {
            name: "SELECTED"
            PropertyChanges {
                target: peg; spriteKey: "selected"
            }
            PropertyChanges {
                target: peg; y: 0
            }

        }
    ]

    transitions: Transition {
        from: "PEG"
        to: "SELECTED"
        SequentialAnimation {
            loops: Animation.Infinite

            NumberAnimation {
                target: peg
                property: "y"
                from: peg.maxHeight; to: peg.minHeight
                easing.type: Easing.InOutQuad; duration: 300
            }

            NumberAnimation {
                target: peg
                property: "y"
                from: peg.minHeight; to: peg.maxHeight
                easing.type: Easing.InOutQuad; duration: 300
            }
        }
    }
    
    CanvasItem {
        id: peg
        spriteKey: currentState
	anchors.fill: parent
	anchors.horizontalCenter: parent.horizontalCenter

        property int maxHeight: -70
        property int minHeight: 0
    }
}
