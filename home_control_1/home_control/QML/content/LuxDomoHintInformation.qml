import QtQuick 1.0
Rectangle
{
    id:hint
    objectName: "hint"
    width: 250
    height: 60
    color: "#af000000"
    property  string   textName: "444"
    property  int        screenH: 480
    LuxDomoLabel
    {
        anchors.fill: parent
        textName: hint.textName
        textSize: 25
        textColor: "white"
    }
    NumberAnimation on opacity
    {
        from:  0;  to:  1; duration: 800
    }
    NumberAnimation on y
    {
        from:  screenH;  to:  screenH*3/5;  duration:1200
    }
}
