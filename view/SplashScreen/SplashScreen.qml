import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

Rectangle {
    id: splashScreen
    width: 400
    height: 400
    color: "transparent"

    // 动态粒子层
    Item {
        anchors.fill: parent
        Repeater {
            model: 15
            Rectangle {
                width: 16; height: 16
                radius: width / 2
                color: Qt.rgba(Math.random(), 0.7, 1.0, 0.12 + Math.random()*0.18)
                x: Math.random() * splashScreen.width
                y: Math.random() * splashScreen.height
                SequentialAnimation on y {
                    loops: Animation.Infinite
                    NumberAnimation { to: splashScreen.height; duration: 3400 + Math.random()*1200; easing.type: Easing.InOutQuad }
                    NumberAnimation { to: 0; duration: 3400 + Math.random()*1200; easing.type: Easing.InOutQuad }
                }
                SequentialAnimation on x {
                    loops: Animation.Infinite
                    NumberAnimation { to: splashScreen.width; duration: 4200 + Math.random()*1200; easing.type: Easing.InOutQuad }
                    NumberAnimation { to: 0; duration: 4200 + Math.random()*1200; easing.type: Easing.InOutQuad }
                }
            }
        }
    }

    // Logo文字
    Text {
        id: logoText
        text: "LabNexus"
        font.pixelSize: 48
        font.bold: true
        font.family: "Orbitron, 'Segoe UI', 'Arial Black', Arial, sans-serif"
        color: "#00eaff"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 64
    }
    // 简单阴影
    Text {
        text: "LabNexus"
        font.pixelSize: 48
        font.bold: true
        font.family: "Orbitron, 'Segoe UI', 'Arial Black', Arial, sans-serif"
        color: "#00223388"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 68
        z: logoText.z - 1
    }

    // 渐变光线条
    Rectangle {
        width: parent.width * 0.7
        height: 12
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: logoText.bottom
        anchors.topMargin: 10
        radius: 6
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#00eaff00" }
            GradientStop { position: 0.5; color: "#00eaff" }
            GradientStop { position: 1.0; color: "#00eaff00" }
        }
        opacity: 0.7
    }

    // 动态科技进度环
    Item {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: 120
        height: 120

        Shape {
            anchors.centerIn: parent
            width: parent.width; height: parent.height
        }

        NumberAnimation {
            id: loaderAnim
            property: "currentValue"
            from: 0
            to: 360
            duration: 2200
            running: true
            loops: Animation.Infinite
        }
    }

    Text {
        text: "Initializing LabNexus..."
        font.pixelSize: 18
        color: "#ffffff"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 48
        opacity: 0.85
    }
}