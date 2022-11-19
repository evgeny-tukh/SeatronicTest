import QtQuick 2.0

Item {
    property int rpm: 0
    property int validity: RpmWidget.Validity.Invalid

    Canvas {
        visible: validity !== RpmWidget.Validity.Invalid
        width: 50; height: 30; x: 145
        y: {
            return verticalPosByRpm (rpm)
        }

        onPaint: {
            var ctx = getContext ("2d")
            ctx.lineWidth = 2
            ctx.strokeStyle = "white"
            ctx.fillStyle = "white"
            ctx.beginPath ()
            ctx.moveTo (0, 12)
            ctx.lineTo (20, 0)
            ctx.lineTo (50, 0)
            ctx.lineTo (50, 24)
            ctx.lineTo (20, 24)
            ctx.moveTo (0, 12)
            ctx.closePath ()
            ctx.fill ()
            ctx.stroke ()
        }

        Text {
            text: "З"
            x: 30; y: 0
            font.pixelSize: 20
            font.weight: Font.Bold
        }
    }
}
