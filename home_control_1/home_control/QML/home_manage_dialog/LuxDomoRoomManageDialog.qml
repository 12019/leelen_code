import  QtQuick 1.0
import  LuxDomoRoomManage  1.0

Rectangle
{
    id:roomManage
    objectName: "roomManage"
    width: 800
    height: 480
    color: "#e47878"

    property string  roomObjectName: ""
    function   addDevice(type,address,attribute,name){  pLuxDomoRoomManageItem.addDevice(type,address,attribute,name) }

    MouseArea
    {
          id:mouseArea
          objectName: "mouseArea"
          anchors.fill: roomManage
          onClicked: {  console.log("roomManageroomManageroomManageroomManageroomManage")  }
    }

    BorderImage
    {
        id: image
        objectName: "image"
        width: parent.width
        height: parent.height*7/8
        anchors.bottom: parent.bottom
        source: "../home_manage/room_background.png"
    }

    LuxDomoRoomManageItem
    {
          id:pLuxDomoRoomManageItem
          objectName: "pLuxDomoRoomManageItem"
    }
}

