import QtQuick 1.0

Rectangle {
    id:scenePage
    width: 460
    height: 401  //240
    color: "#00000000"
    radius: 10
    objectName: "scenePage"

    property string sceneBG : "images/home_control/scene/bg.png"
    property int currentPressX: 0
    property int currentIndex : 0

    LocalFontLoader{
        id:localfont
    }

    Image {
        id: bor
        source: sceneBG
        anchors.fill: parent
    }

    function setState(mouseX){
        //console.log(mouseX)
        if(mouseX - currentPressX <= -15){
            currentIndex = 1
            scenePage.state = "sceneDlgLeft"
        }
        if(mouseX - currentPressX >= 15){
            currentIndex = 0
            scenePage.state = "sceneDlg"
        }
    }

    MouseArea{
        anchors.fill: parent
        onPressed: {
            currentPressX = mouseX
        }
        onPositionChanged: {
            setState(mouseX)
        }
    }

    HomeControlSceneDialog{
        id:sceneDlg
        width: scenePage.width
        height: scenePage.height
        visible: x == 0?true:false
    }
    Page1{
        id:page1
        width: scenePage.width
        height: scenePage.height
        x:scenePage.width
        visible: x == 0?true:false
    }

    /*VisualItemModel {
        id: itemModel
        HomeControlSceneDialog{
            id:sceneDlg
            width: scenePage.width
            height: scenePage.height
        }
        Page1{
            id:page1
            width: scenePage.width
            height: scenePage.height
        }
    }

    ListView {
        property  int flag: 1
        highlightMoveSpeed: 999
        id: view
        flickDeceleration: 1750
        maximumFlickVelocity: 2500
        anchors { fill: parent; }
        model: itemModel
        preferredHighlightBegin: 0
        preferredHighlightEnd: 0
        highlightRangeMode: ListView.StrictlyEnforceRange
        orientation: ListView.Horizontal
        snapMode: ListView.SnapOneItem
        clip: true
        onMovementStarted: {
            flag = 0
            console.log("move start")
        }
    }*/

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 20
        anchors.top: parent.top
        anchors.topMargin: 15

        Repeater {
            model: 2

            Rectangle {
                width: 5
                height: 5
                radius: 3
                color: currentIndex == index ? "black" : "gray"

                /*MouseArea {
                    width: 20; height: 20
                    anchors.centerIn: parent
                    //onClicked: view.currentIndex = index
                }*/
            }
        }
    }

    states: [
        State {
            name: "sceneDlgLeft"
            PropertyChanges {
                target: sceneDlg
                x:-scenePage.width
            }
            PropertyChanges {
                target: page1
                x: 0
                visible:true
            }
        },
        State {
            name: "sceneDlg"
            PropertyChanges {
                target: sceneDlg
                x:0
                visible:true
            }
            PropertyChanges {
                target: page1
                x: scenePage.width
                visible:false
            }
        }
    ]
    transitions: Transition {
        NumberAnimation{ properties: "x,y";easing.type: Easing.InOutSine;duration:500}
    }
}
