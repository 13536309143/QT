import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Course
ApplicationWindow {
    width: 480; height: 320; visible: true; title: "Qt Quick + C++"
    Counter { id: backend }
    ColumnLayout {
        anchors.centerIn: parent; spacing: 20
        Label { text: "点击次数：" + backend.count; font.pixelSize: 28 }
        Button { text: "增加"; onClicked: backend.increment() }
    }
}
