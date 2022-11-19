import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.1

import "RpmWidget"

ApplicationWindow {
    id: mainWnd;
    visible: true
    width: 400//200
    height: 650
    title: qsTr("Browser")
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
    color: "gray"
    property bool reallyClosingNow: false

    MessageDialog {
        id: exitConfDlg
        icon: StandardIcon.Question
        text: "Are you sure to exit?"
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: {
            reallyClosingNow = true
            mainWnd.close ()
        }
    }

    onClosing: {
        close.accepted = reallyClosingNow;
        if (!reallyClosingNow) {
            exitConfDlg.open ();
        }
    }

    RpmWidget {
        id: rpmWidget
        rpm: 223
        validity: RpmWidget.Validity.Valid
    }

    Text {
        x: 220; y: 10
        text: "Test console"
        font.pixelSize: 16;
    }

    Rectangle {
        x: 200; y: 40; width: 200
        Rectangle {
            x: 5; y: 40; width: 200
            color: "black"
            Label {
                font.pixelSize: 16;
                x: 2; y: 0
                text: "RPM"
            }
            TextInput {
                id: rpmField
                x: 60; y: 0; width: 80; height: 25
                font.pixelSize: 16;
                inputMask: "#009"
                text: "158"
                padding: 1
                leftPadding: 2
                readOnly: false
                onTextChanged: {
                    var newRpm
                    try {
                        newRpm = parseInt (text.trim())
                    } catch (error) {
                        return;
                    }
                    rpmWidget.rpm = newRpm
                }
            }
        }
        Rectangle {
            x: 5; y: 70; width: 200
            Label {
                x: 2; y: 2
                text: "Validity"
                font.pixelSize: 16;
            }
            ComboBox {
                id: curValidity
                x: 60; y: 0; width: 120; height: 24
                font.pixelSize: 14;
                editable: false
                padding: 0
                leftPadding: 2
                model: ["Valid", "Outdated", "Unreliable", "Invalid"]
                onCurrentIndexChanged: {
                    var newValidity = currentIndex + 1
                    rpmWidget.validity = newValidity
                }
            }
        }
    }
}

