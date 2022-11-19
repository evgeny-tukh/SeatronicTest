import QtQuick 2.0

Text {
    id: root
    color: "white"
    font.pixelSize: 18
    property int rpm: 0
    text: rpm
    width: 40; height: 20; x: 10
    y: verticalTickLabelPosByRpm (rpm)
}
