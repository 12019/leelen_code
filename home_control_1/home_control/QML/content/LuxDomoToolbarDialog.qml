import   QtQuick 1.0
import   LuxDomoToolbar  1.0
Rectangle
{
    id:toolbar
    objectName: "toolbar"
    width: 800
    height: 60
    color:"#af000000"

    property    string    title: ""
    property    bool      flag: true

    LuxDomoToolbarItem
    {
          id:pLuxDomoToolbarItem
          objectName: "pLuxDomoToolbarItem"
    }

    Image
    {
        anchors.fill: parent
        source: "../home_manage/top_background.png"
    }

    Item
    {
        width: parent.width/5
        height: parent.height
        anchors.left: parent.left
        visible: flag
        enabled: flag
        Image
        {
              id:backImage
              width:parent.width/6
              height: parent.height*3/5
              anchors.verticalCenter: parent.verticalCenter
              anchors.left: parent.left
              anchors.leftMargin: parent.width/8
              source: "../home_manage/back_off.png"
        }
        LuxDomoLabel
        {
            width: parent.width/3
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: backImage.right
            textName: "返回"
            textSize:20
            color:"#00000000"
        }
        MouseArea
        {
            anchors.fill: parent
            onClicked: {   pLuxDomoToolbarItem.back()   }
        }
    }

    LuxDomoLabel
    {
        width: parent.width/4
        height: parent.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        textName: toolbar.title
        textSize: 25
    }
}

