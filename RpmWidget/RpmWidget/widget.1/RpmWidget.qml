import QtQuick 2.0

Item {
    id: root

    enum Validity {
        Valid = 1,
        Outdated = 2,
        Unreliable = 3,
        Invalid = 4
    }

    property int rpm: 0
    property int validity: RpmWidget.Validity.Invalid

    function curValidity () {
        return 1;
    }

    // pecentage should be between -100 and 100
    function verticalOffsetByPercentage (percentage) {
        return 23 + (widget.height - 150) / 200 * (100 - percentage)
    }

    // rpm should be between -250 and 250; -250 is -100 and 250 is +100
    function verticalOffsetByRpm (rpm, rpmMaxPos, rpmMaxNeg) {
        var percentage;
        if (rpm > rpmMaxPos) {
            rpm = rpmMaxPos
        } else if (rpm < rpmMaxNeg) {
            rpm = rpmMaxNeg
        }
        if (rpm > 0) {
            percentage = rpm * 100 / rpmMaxPos
        } else {
            percentage = - rpm * 100 / rpmMaxNeg
        }
        return verticalOffsetByPercentage (percentage)
    }

    function verticalPosByRpm (rpm) {
        return verticalOffsetByRpm (rpm, 250, -250)
    }

    function verticalTickLabelPosByRpm (rpm) {
        return verticalPosByRpm (rpm) - 3
    }

    Rectangle {
        border.color: "black"
        width: 200; height: root.parent.height
        color: "#222222"

        Gauge { rpm: root.rpm; validity: root.validity }
        RpmDisplay { rpm: root.rpm; validity: root.validity }
        GraphicalDisplay { rpm: root.rpm; validity: root.validity }
        TickTags {}
    }
}
