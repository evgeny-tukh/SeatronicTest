import QtQuick 2.0

Rectangle {
    id: root
    property bool doubleWidth: false
    x: 0; width: 80; height: doubleWidth ? 4 : 2
    border.width: doubleWidth ? 2 : 1
    border.color: "white"
    property int rpm: 0
    y: verticalPosByRpm (rpm) + (doubleWidth ? 0 : 1)
}
