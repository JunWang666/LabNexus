import QtQuick 6.0
import QtQuick.Controls 6.0

Rectangle {
    id: splashScreen
    width: 400
    height: 400
    color: "transparent"

    // 谷歌风格几何图形背景（空心/边框，无缝横向慢速滚动）
    Item {
        id: geoBackground
        anchors.fill: parent

        property int cols: 8
        property int rows: 7
        property real offset: 0

        NumberAnimation on offset {
            from: 0
            to: splashScreen.width
            duration: 60000
            running: true
            loops: Animation.Infinite
        }

        Repeater {
            model: (geoBackground.cols + 1) * geoBackground.rows
            delegate: Item {
                property int col: index % (geoBackground.cols + 1)
                property int row: Math.floor(index / (geoBackground.cols + 1))
                width: splashScreen.width / geoBackground.cols
                height: splashScreen.height / geoBackground.rows
                // 横向无缝平移，y不变
                x: ((col * width - geoBackground.offset) % splashScreen.width)
                y: row * height

                property int shapeType: (row + col) % 3

                Canvas {
                    anchors.fill: parent
                    opacity: 0.28
                    onPaint: {
                        var ctx = getContext("2d")
                        ctx.reset()
                        var w = width
                        var h = height
                        ctx.lineWidth = 2.2
                        ctx.strokeStyle = (parent.shapeType === 0) ? "#2196f3" :
                            (parent.shapeType === 1) ? "#43a047" : "#fbc02d"
                        ctx.globalAlpha = 0.8
                        if (parent.shapeType === 0) { // 六边形
                            ctx.beginPath()
                            for(var i=0; i<6; ++i) {
                                var angle = Math.PI/3 * i - Math.PI/2
                                var x = w/2 + Math.cos(angle)*w*0.36
                                var y = h/2 + Math.sin(angle)*h*0.36
                                if(i===0) ctx.moveTo(x,y)
                                else ctx.lineTo(x,y)
                            }
                            ctx.closePath()
                            ctx.stroke()
                        } else if (parent.shapeType === 1) { // 三角形
                            ctx.beginPath()
                            ctx.moveTo(w*0.15, h*0.85)
                            ctx.lineTo(w*0.5, h*0.18)
                            ctx.lineTo(w*0.85, h*0.85)
                            ctx.closePath()
                            ctx.stroke()
                        } else { // 圆形
                            ctx.beginPath()
                            ctx.arc(w/2, h/2, w*0.33, 0, Math.PI*2)
                            ctx.closePath()
                            ctx.stroke()
                        }
                    }
                }
            }
        }
    }

    // Logo动画：从左到右出现，再从左到右消失，循环
    Row {
        id: logoRow
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 110
        spacing: 0

        property int animIndex: 0
        property int fadeIndex: -1
        property bool fading: false

        function restartAnimation() {
            animIndex = 0
            fading = false
            fadeIndex = -1
            letterAnim.start()
        }

        Component.onCompleted: restartAnimation()

        Repeater {
            id: letterRepeater
            model: "LabNexus".length
            delegate: Item {
                width: 38
                height: 60
                property int letterIdx: index

                Text {
                    id: letterText
                    text: "LabNexus".charAt(index)
                    font.pixelSize: 44
                    font.bold: true
                    font.family: "Segoe UI, Arial, sans-serif"
                    color: "#1673ff"
                    // opacity: (logoRow.fading
                    //     ? (logoRow.fadeIndex > letterIdx ? 0
                    //         : logoRow.fadeIndex === letterIdx ? fadeAnim.currentValue : 1)
                    //     : (logoRow.animIndex >= letterIdx ? 1 : 0))
                    // scale: (logoRow.animIndex === letterIdx && !logoRow.fading) ? scaleAnim.currentValue : 1
                    scale: (logoRow.animIndex === letterIdx && !logoRow.fading) ? (scaleAnim.currentValue || 1.0) : 1.0
                    opacity: (logoRow.fading
                        ? (logoRow.fadeIndex > letterIdx ? 0
                            : logoRow.fadeIndex === letterIdx ? (fadeAnim.currentValue || 1.0) : 1.0)
                        : (logoRow.animIndex >= letterIdx ? 1.0 : 0))
                    anchors.centerIn: parent

                    NumberAnimation on scale {
                        id: scaleAnim
                        from: 1.0
                        to: 1.18
                        duration: 120
                        easing.type: Easing.OutBack
                        running: logoRow.animIndex === letterIdx && !logoRow.fading
                        // onStopped: scale = 1.0
                    }

                    NumberAnimation on opacity {
                        id: fadeAnim
                        from: 1
                        to: 0
                        duration: 70
                        running: logoRow.fading && logoRow.fadeIndex === letterIdx
                    }
                }
            }
        }

        Timer {
            id: letterAnim
            interval: 80 // 速度调节
            repeat: true
            running: false
            onTriggered: {
                if (!logoRow.fading) {
                    if (logoRow.animIndex < "LabNexus".length-1) {
                        logoRow.animIndex += 1
                    } else {
                        logoRow.fading = true
                        logoRow.fadeIndex = 0
                    }
                } else {
                    if (logoRow.fadeIndex < "LabNexus".length-1) {
                        logoRow.fadeIndex += 1
                    } else {
                        logoRow.restartAnimation()
                    }
                }
            }
        }
    }

    Rectangle {
        width: parent.width * 0.6
        height: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: logoRow.bottom
        anchors.topMargin: 12
        radius: 6
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#1673ff00" }
            GradientStop { position: 0.5; color: "#1673ff88" }
            GradientStop { position: 1.0; color: "#1673ff00" }
        }
        opacity: 0.6
    }

    Text {
        text: "Initializing LabNexus..."
        font.pixelSize: 16
        color: "#ffffff"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 48
        opacity: 0.82
    }
}