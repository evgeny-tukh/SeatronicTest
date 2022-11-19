import QtQuick 2.0

Item {
    id: root
    property bool doubleWidth: false
    property int rpm: 0

    Rectangle {
        x: 0; width: 80; height: root.doubleWidth ? 4 : 2
        border.width: doubleWidth ? 2 : 1
        border.color: "white"
        y: verticalPosByRpm (root.rpm) + (root.doubleWidth ? 0 : 1)
    }
}
