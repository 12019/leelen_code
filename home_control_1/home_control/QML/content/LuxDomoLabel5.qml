import QtQuick 1.0
Rectangle
{
    id:label
    objectName: "label"
    width: 100
    height: 100
    color: "#00000000"

    property   string   textName:""
    property   int        textSize: 20
    property   string   textColor: "#151412"

    LuxDomoFontLoader
    {
        id:luxdomoLoader
    }

    TextEdit
    {
        id:text
        objectName: "text"
        width: parent.width
        height: parent.height
        font.family: luxdomoLoader.name
        wrapMode: TextEdit.WordWrap
        text:textName
        color: textColor
        font.pixelSize: textSize
        readOnly: true
    }
}
