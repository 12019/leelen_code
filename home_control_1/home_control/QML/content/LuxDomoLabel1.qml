import QtQuick 1.0
Rectangle
{
    id:label
    objectName: "label"
    width: 100
    height: 100
    color: "#00000000"

    property   string   textName: "0"
    property   int        textSize: 20
    property   string   textColor: "#151412"
    property   int        margin:0

    LuxDomoFontLoader
    {
        id:luxdomoLoader
    }

    TextEdit
    {
        id:text
        objectName: "text"
        anchors.verticalCenter:parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: margin
        font.family: luxdomoLoader.name
        text:textName
        color: textColor
        font.pixelSize: textSize
        readOnly: true
        //clip: true
        //elide: Text.ElideMiddle
    }
}
