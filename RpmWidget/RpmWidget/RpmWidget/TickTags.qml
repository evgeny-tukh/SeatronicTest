import QtQuick 2.0

Item {
    Rectangle {
        border.color: "transparent"
        border.width: 0
        width: 40; height: parent.height - 150 + 10; x: 0; y: 6
        color: "transparent"

        TickTag { rpm: 250 }
        TickTag { rpm: 200 }
        TickTag { rpm: 150 }
        TickTag { rpm: 100 }
        TickTag { rpm: 50 }
        TickTag { rpm: 0 }
        TickTag { rpm: -50 }
        TickTag { rpm: -100 }
        TickTag { rpm: -150 }
        TickTag { rpm: -200 }
        TickTag { rpm: -250 }
    }
}
