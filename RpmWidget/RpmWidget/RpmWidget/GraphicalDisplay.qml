import QtQuick 2.0

Item {
    id: root;
    property int rpm: 0
    property int validity: RpmWidget.Validity.Invalid
    property string positiveFillColor: "yellow"
    property string negativeFillColor: "lightBlue"

    Rectangle {
        border.color: "white"
        border.width: 3
        width: 80; height: 556; x: 60; y: 13
        radius: 8
        color: "transparent"
        ValueIndicator {
            id: positiveArea
            rpm: root.rpm
            color: root.positiveFillColor
            positive: true
            validity: root.validity
        }
        ValueIndicator {
            id: negativeArea
            rpm: root.rpm
            color: root.negativeFillColor
            positive: false
            validity: root.validity
        }
        TickMarks {}
    }
}
