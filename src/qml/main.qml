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

Item {
    id: main
    property string state
    property string boardStyle

    CanvasItem {
        id: background
        spriteKey: "background"
        anchors.fill: parent
    }
    
    Item {
        id: timerItem

        property int seconds: 0
        property bool enabled: false

        anchors.top: main.top
        anchors.topMargin: 30
        x: (parent.width - time.width) / 2

        Timer {
	    id: timer
            interval: 1000
	    running: true
	    repeat: true
            onTriggered: {
                if (timerItem.enabled) timerItem.seconds++
            }
        }

        Text {
            id: time
            text: i18n("Time: ") + getTime(timerItem.seconds)
            color: "black"
	    visible: false
            font.pixelSize: 20
            font.bold: true
        }
    }
    
    Rectangle {
        id: gameCanvas
        property int pegSize: 70
        
        color: "transparent"
        anchors.centerIn: background

        MouseArea {
            id: gameCanvasMouseArea
            anchors.fill: gameCanvas
            z: 10
        }
    }
}
