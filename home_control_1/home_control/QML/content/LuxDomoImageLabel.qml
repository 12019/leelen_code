import QtQuick 1.0
Rectangle
{
    id:imageLabel
    objectName: "imageLabel"
    width: 100
    height: 40
    color: "#00000000"

    property   string   textName: "0"
    property   int        textSize: 17
    property   string   textColor: "#151412"
    property   string   imageStr: ""

    LuxDomoFontLoader
    {
        id:luxdomoLoader
    }

    TextEdit
    {
        id:textStr
        objectName: "textStr"
        anchors.verticalCenter:imageLabel.verticalCenter
        x:parent.width/2
        font.family: luxdomoLoader.name
        text:imageLabel.textName
        color: imageLabel.textColor
        font.pixelSize: imageLabel.textSize
        readOnly: true
    }

    Image
    {
        width: parent.height
        height: parent.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: textStr.left
        source: imageStr
    }
}

