import QtQuick 1.0
import   LuxDomoRoomContainer 1.0
import  "../content"
Rectangle
{
        id:roomContaniner
        objectName: "roomContaniner"
        width: 800
        height: 480
        color: "#00000000"

        property int   addRoomX: roomContaniner.width
        property int   addRoomY: roomContaniner.height
        property int   addDeviceW: 80
        property int   addDeviceH: 110
        function   init(){  pLuxDomoRoomContainerItem.init()  }
        function   showFunctionOption(roomObjectName) {  pLuxDomoRoomContainerItem.showFunctionOption(roomObjectName)  }
        function   deleteRoom() { pLuxDomoRoomContainerItem.deleteRoom()   }
        function   setRoomName(roomName){ pLuxDomoRoomContainerItem.setRoomName(roomName)  }
        property   int  screenH: 480
        property   int  nameSize: 20

        LuxDomoRoomContainerItem
        {
              id:pLuxDomoRoomContainerItem
             objectName: "pLuxDomoRoomContainerItem"
        }

        onYChanged:
        {
                if(roomContaniner.y>0)  {  roomContaniner.y=0   }
                else  if(-roomContaniner.y>roomContaniner.screenH-600)  {  roomContaniner.y=screenH-roomContaniner.height   }
        }

        MouseArea
        {
               id:mouseArea
               objectName: "mouseArea"
               anchors.fill: parent
               drag.target: roomContaniner
               drag.axis: Drag.YAxis
        }     

        Rectangle
        {
              id:addRoom
              objectName: "addRoom"
              height:addDeviceH
              width: addDeviceW
              x:roomContaniner.addRoomX
              y:roomContaniner.addRoomY
              color: "#00000000"

              LuxDomoButton
              {
                     id:addButton
                     height: addRoom.width
                     width: addRoom.width
                     anchors.top: addRoom.top
                     anchors.left: addRoom.left
                     image: "../home_manage/add_on.png"
                     imagepressed: "../home_manage/add_on.png"
                     buttonColor: "#00000000"
                     onClicked: {  pLuxDomoRoomContainerItem.addRoom()  }
               }

              LuxDomoLabel
              {
                  id:textRec
                  objectName: "textRec"
                  width: addRoom.width
                  height:  addRoom.height-addRoom.width
                  anchors.bottom: addRoom.bottom
                  textName:"添加房间"
                  textSize: nameSize
              }
        }
}

