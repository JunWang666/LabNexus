// SplashScreen.qml
import QtQuick
import QtQuick.Window

Rectangle {
    // 设置根元素的ID，方便调试或引用
    id: splashScreenRoot

    // 填充整个屏幕或父容器
    width: parent.width > 0 ? parent.width : 400 // 如果parent有宽度就用parent的，否则给个默认值
    height: parent.height > 0 ? parent.height : 300 // 同上

    // 背景颜色
    color: "#3498db" // 一个友好的蓝色

    // 在中心显示文本
    Text {
        id: welcomeText
        text: "欢迎来到我的应用！"
        font.pixelSize: 24
        color: "white"
        anchors.centerIn: parent // 文本在父容器中居中

        // 简单的动画效果（可选，但能看出界面是否活跃）
        Behavior on opacity {
            NumberAnimation { duration: 1000 }
        }
        opacity: 0
        // 加载后渐显
        Component.onCompleted: {
            opacity = 1
        }
    }
}