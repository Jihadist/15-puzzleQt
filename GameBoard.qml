import QtQuick 2.0
import Game 1.0

GridView {
    id: root

    signal gameStateChanged(bool state)

    function f(s) {
        console.log("score is: ", s)
    }

    function setState(state) {
        root.visible = state
        root.enabled = state
    }

    cellHeight: height / 4
    cellWidth: width / 4

    move: Transition {
        NumberAnimation {
            properties: "x, y"
            duration: 200
        }
    }
    delegate: Item {
        id: _backgroundDelegate
        width: root.cellWidth
        height: root.cellHeight

        visible: root.model.hiddenElementValue !== display

        Tile {

            anchors.fill: _backgroundDelegate
            anchors.margins: 5

            internalText.text: display.toString()
            MouseArea {
                anchors.fill: parent
                onClicked: {

                    root.model.move(index)
                }
            }
        }
    }
    GameController_qml {
        id: _gameController
    }

    Component.onCompleted: {
        root.model = _gameController.getModel()

        root.gameStateChanged.connect(setState)
        root.model.stateChanged.connect(gameStateChanged)
    }
}
