import QtQuick 2.0

Item {
    id: root
    property bool positive: true;
    property int rpm: 0
    property string color
    property int validity: RpmWidget.Validity.Invalid

    Rectangle {
        visible: (rpm > 0) === positive && root.validity !== RpmWidget.Validity.Invalid;
        border.width: 0
        border.color: "transparent"
        width: 74; x: 3
        y: positive ? verticalPosByRpm (root.rpm) : verticalPosByRpm (0)
        height: positive ? 2 + (verticalPosByRpm (0) - verticalPosByRpm (root.rpm)) : (verticalPosByRpm (root.rpm) - verticalPosByRpm (0)) - 2
        color: root.color
        radius: 2
    }
}
