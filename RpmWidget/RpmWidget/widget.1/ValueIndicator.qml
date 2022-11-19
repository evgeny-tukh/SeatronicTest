import QtQuick 2.0

Item {
    property bool positive: true;
    property int rpm: 0
    property string color
    property int validity: 1

    id: root

    Rectangle {
        visible: (rpm > 0) == positive;
        border.width: 0
        border.color: "transparent"
        width: 74; x: 3
        y: positive ? verticalPosByRpm (rpm) : verticalPosByRpm (0)
        height: positive ? 2 + (verticalPosByRpm (0) - verticalPosByRpm(rpm)) : (verticalPosByRpm(rpm) - verticalPosByRpm(0)) - 2
        color: root.color
        radius: 2
    }
}
