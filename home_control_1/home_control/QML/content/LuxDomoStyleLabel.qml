import QtQuick 1.0
Rectangle
{
    id:styleLabel
    objectName: "styleLabel"
    width: 100
    height: 30
    color: "#00000000"

    signal  clicked
    property   string   textName: "44444"
    property   int        textSize: 20
    property   string   textColor: "#151412"
    property   string   textImage: ""

    TextEdit
    {
        id:text
        objectName: "text"
        anchors.verticalCenter:parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        text:styleLabel.textName
        color: styleLabel.textColor
        font.pixelSize: styleLabel.textSize
        readOnly: true
    }

    Image
    {
        height: parent.height/3
        width: parent.height/3
        anchors.verticalCenter:parent.verticalCenter
        anchors.left: text.right
        source: styleLabel.textImage
    }

    MouseArea
    {
        anchors.fill: parent
        onClicked: {  styleLabel.clicked()  }
    }
}

