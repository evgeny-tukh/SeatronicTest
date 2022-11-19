import QtQuick 2.0

Item {
    id: root
    property int rpm: 0

    Text {
        color: "white"
        font.pixelSize: 18
        text: rpm
        width: 40; height: 20; x: 10
        y: verticalTickLabelPosByRpm (root.rpm)
    }
}
