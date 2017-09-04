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
    property   string   textName1: "0"
    property   bool     textElide: false
    property   int        totalLength: 5
    property   int        showLength: 3


    onTextNameChanged:
    {
        if(textName.length<=totalLength)
        {
            textName1=textName
        }
        else
        {
            textName1=textName.substring(0,showLength)+"..."
        }
    }

    LuxDomoFontLoader
    {
        id:luxdomoLoader
    }

    TextEdit
    {
        id:text
        objectName: "text"
        anchors.verticalCenter:label.verticalCenter
        anchors.horizontalCenter: label.horizontalCenter
        font.family: luxdomoLoader.name
        text:textElide?label.textName1:label.textName
        color: label.textColor
        font.pixelSize: label.textSize
        readOnly: true
    }
}
