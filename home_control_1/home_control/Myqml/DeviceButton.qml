import QtQuick 1.0

Rectangle {
    id:pre_next
    width: textwidth+100//200
    height: 55
    color:"#00000000"
    property string msg: "test"
    property int textwidth: 100
    signal preclicked
    signal nextclicked
    signal prePressed
    signal nextPressed
    signal buttonReleased

    Button {
        id: pre
        x: 0
        y: 0
        width: 50
        height: 55
        image:"images/home_control/device/1.png"
        imagepressed: "images/home_control/device/1-1.png"
        color: "#00000000"
        anchors.right: textedit1.left
        anchors.rightMargin: 0
        onClicked: pre_next.preclicked()
        onPressed: pre_next.prePressed()
        onReleased: pre_next.buttonReleased()
    }

    Rectangle{
        x: 50
        y: 0
        width: 100
        height: 48
    }

    TextEdit{
        id: textedit1
        x: 50
        y: 0
        width: pre_next.textwidth
        height: 48
        color: "#000000"
        text: pre_next.msg
        font.family: localfont.name
        font.pixelSize: 18
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        readOnly: true
    }

    Button {
        id: next
        y: 0
        width: 50
        height: 55
        color:"#00000000"
        anchors.left: textedit1.right
        anchors.leftMargin: 0
        image:"images/home_control/device/2.png"
        imagepressed: "images/home_control/device/2-1.png"
        onClicked: pre_next.nextclicked()
        onPressed: pre_next.nextPressed()
        onReleased: pre_next.buttonReleased()
    }
}
