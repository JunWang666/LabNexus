import QtQuick 6.0
import QtQuick.Controls 6.0

Rectangle {
    id: splashScreen
    width: 400
    height: 400
    color: "transparent"

    // 粒子层
    Item {
        anchors.fill: parent
        Repeater {
            model: 15
            Rectangle {
                width: 16
                height: 16
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

    // 动画拼接 LabNexus 字母
    Row {
        id: logoRow
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 80
        spacing: 4

        property int animIndex: 0

        Component.onCompleted: letterAnim.start()

        function letterColor(idx) {
            // 用渐变蓝色
            var colors = ["#30cfff", "#30eaff", "#30ffff", "#30dfff", "#30ceff", "#30bfff", "#30aaff", "#3088ff"]
            return colors[idx % colors.length];
        }

        Repeater {
            id: letterRepeater
            model: "LabNexus".length
            delegate: Item {
                width: 44; height: 60

                // 字母
                Text {
                    id: letterText
                    text: "LabNexus".charAt(index)
                    font.pixelSize: 48
                    font.bold: true
                    font.family: "Orbitron, 'Segoe UI', 'Arial Black', Arial, sans-serif"
                    color: logoRow.letterColor(index)
                    opacity: 0
                    scale: 2.0
                    anchors.centerIn: parent

                    // 入场动画，依次拼接每个字母
                    SequentialAnimation {
                        id: showAnim
                        running: logoRow.animIndex >= index
                        PropertyAnimation { target: letterText; property: "opacity"; from: 0; to: 1; duration: 200 }
                        PropertyAnimation { target: letterText; property: "scale"; from: 2.0; to: 1.0; duration: 320; easing.type: Easing.OutBack }
                        PauseAnimation { duration: 80 }
                        NumberAnimation { target: letterText; property: "scale"; from: 1.0; to: 1.08; duration: 240; easing.type: Easing.InOutQuad; loops: Animation.Infinite; }
                    }
                }
            }
        }

        // 控制字母拼接动画顺序
        Timer {
            id: letterAnim
            interval: 260
            repeat: true
            running: false
            onTriggered: {
                if (logoRow.animIndex < "LabNexus".length-1) {
                    logoRow.animIndex += 1
                } else {
                    // 无限循环拼接动画（可选，如果只拼一次可以注释掉）
                    // logoRow.animIndex = 0
                    // letterAnim.restart()
                    letterAnim.stop()
                }
            }
        }
    }

    // 渐变光线条
    Rectangle {
        width: parent.width * 0.7
        height: 12
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: logoRow.bottom
        anchors.topMargin: 10
        radius: 6
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#00eaff00" }
            GradientStop { position: 0.5; color: "#00eaff" }
            GradientStop { position: 1.0; color: "#00eaff00" }
        }
        opacity: 0.7
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