import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: window
    width: 640
    height: 480
    minimumWidth: 200
    minimumHeight: 250
    visible: true
    title: qsTr("Hello World")

    property bool lightMode: Application.styleHints.colorScheme === Qt.Light
    property color reallyDark: "#1f1f1f"
    property color dark: "#262626"
    property color reallyLight: "#e7e7e7"
    property color light: "#e0e0e0"

    Image {
        id: viddeoImage
        anchors.fill: parent

        source: "image://videoFrame/frame"
                + videoFrameUpdater.frameVersion
        fillMode: Image.PreserveAspectFit

        cache: false //이전 프레임 캐싱 - 재사용 벙지
    }
}