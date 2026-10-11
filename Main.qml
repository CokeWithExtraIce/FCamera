import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window

    width: 960
    height: 720
    minimumWidth: 480
    minimumHeight: 360
    visible: true
    title: qsTr("FCamera")

    property color backgroundColor: "#1f1f1f"
    property color cellColor: "#262626"
    property color borderColor: "#454545"
    property color textColor: "#e7e7e7"

    component VideoCell: Rectangle {
        id: cell

        required property int cellId
        required property var updater
        property string waitingMessage: "Waiting for first frame..."

        color: window.cellColor
        border.color: window.borderColor
        border.width: 1
        clip: true

        Image {
            id: videoImage

            anchors.fill: parent

            source: cell.updater.streamingInit
                   ? "image://videoFrame/"
                     + cell.cellId + "/"
                     + cell.updater.frameVersion
                   : ""

            fillMode: Image.PreserveAspectFit
            cache: false
            asynchronous: false
        }

        Text {
            anchors.centerIn: parent

            visible: !cell.updater.streamingInit
            text: cell.waitingMessage

            color: window.textColor
            font.pixelSize: 18
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 8

            text: "Cell " + cell.cellId
            color: window.textColor
            font.pixelSize: 14
        }
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: 8

        columns: 2
        rows: 2
        rowSpacing: 8
        columnSpacing: 8

        VideoCell {
            Layout.fillWidth: true
            Layout.fillHeight: true

            cellId: 0
            updater: cell0Updater
        }

        VideoCell {
            Layout.fillWidth: true
            Layout.fillHeight: true

            cellId: 1
            updater: cell1Updater
        }

        VideoCell {
            Layout.fillWidth: true
            Layout.fillHeight: true

            cellId: 2
            updater: cell2Updater
            waitingMessage: "Waiting for scheduler..."
        }

        VideoCell {
            Layout.fillWidth: true
            Layout.fillHeight: true

            cellId: 3
            updater: cell3Updater
            waitingMessage: "Waiting for scheduler..."
        }
    }
}