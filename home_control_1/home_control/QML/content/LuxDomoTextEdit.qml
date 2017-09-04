import QtQuick 1.0
Rectangle
{
    id:textTec
    objectName: "textTec"
    width: 100
    height: 62

    signal  clicked
    property   string    textName: ""
    property   int         textSize: 20
    property   string    textColor: "red"

    TextEdit
    {
        anchors.horizontalCenter: button.horizontalCenter
        anchors.verticalCenter: button.verticalCenter
        text:textTec.textName
        color: textTec.textColor
        font.pixelSize: textTec.textSize
    }
    MouseArea
    {
        anchors.fill: parent
        onClicked: textTec.clicked()
   }
}

