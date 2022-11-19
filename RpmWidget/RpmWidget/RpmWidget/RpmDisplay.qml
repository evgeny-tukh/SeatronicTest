import QtQuick 2.0

Item {
    id: root;
    property int rpm: 0
    property int validity: RpmWidget.Validity.Invalid

    Text {
        text: "об/мин"
        horizontalAlignment: Text.AlignHCenter
        color: "#DDDDDD"
        width: 200; height: 20; x: 0; y: root.parent.height - 70;
        font.pixelSize: 16
    }
    Text {
        id: rpmValue
        text: root.validity === RpmWidget.Validity.Invalid ? "Н/Д" : rpm
        horizontalAlignment: Text.AlignHCenter
        color: {
            switch (root.validity) {
                case RpmWidget.Validity.Valid: return "white";
                case RpmWidget.Validity.Outdated: return "gray";
                default: return "red";
            }
        }
        width: 200; height: 20; x: 0; y: root.parent.height - 50;
        font.pixelSize: 22
        font.weight: Font.Bold
    }
}
