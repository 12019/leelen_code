import QtQuick 1.0
Rectangle
{
    id:label
    objectName: "label"
    width: 100
    height: 100
    color: "#00000000"

    property   string   textName: "0"
    property   int        textSize: 17
    property   string   textColor: "#151412"

    LuxDomoFontLoader
    {
        id:luxdomoLoader
    }

    TextEdit
    {
        id:text
        objectName: "text"
        anchors.bottom: label.bottom
        anchors.horizontalCenter: label.horizontalCenter
        font.family: luxdomoLoader.name
        text:label.textName
        color: label.textColor
        font.pixelSize: label.textSize
        readOnly: true
    }
}
