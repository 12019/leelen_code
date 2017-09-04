import QtQuick 1.0

Rectangle {
    id:eastTouch
    width: 80
    height: 80
    color: "#00000000"
    signal clicked

    property string easyTouchmune_1 : "images/home_control/scene/mune-1.png"
    property string easyTouchmune : "images/home_control/scene/mune.png"

    function setPressIcon(){
        easytouch.source = easyTouchmune_1//"images/home_control/scene/mune-1.png"
    }
    function setReleaseIcon(){
        easytouch.source = easyTouchmune//"images/home_control/scene/mune.png"
    }
    Image {
        id: easytouch
        /*source: easyMouse.pressed?
        "images/home_control/scene/mune-1.png":
        "images/home_control/scene/mune.png"// This is available in all editors.*/
        source: easyMouse.pressed?
        easyTouchmune_1:
        easyTouchmune
        anchors.fill: parent
    }
    MouseArea{
        id:easyMouse
        anchors.fill: parent
        drag.target: eastTouch
        drag.axis: Drag.XandYAxis
        onPressed: {
            //previousPosition = Qt.point(mouseX, mouseY)
        }
        onClicked: {
            eastTouch.clicked()
        }
        onReleased: {
            //mainRec.state = "easyTouchMove"
        }

    }
}
