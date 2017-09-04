import  QtQuick 1.0
import   LuxDomoRoom  1.0
import   "../content"
Rectangle
{
          id:room
          objectName: "room"
          width: 80
          height: 100
         color: "#00000000"

          property  string  roomName: ""
          property  string  roomImage: ""
          property  bool    isMoveParent: false
          property  int       oldY: 0
          property  int       nameSize: 20

          LuxDomoRoomItem
          {
                 id:pLuxDomoRoomItem
                 objectName: "pLuxDomoRoomItem"
          }

          Rectangle
          {
                id:image
                objectName: "image"
                width: room.width
                height: room.width
                anchors.top: room.top
                anchors.left: room.left
                color: "#00000000"
                Image
                   {
                         anchors.fill: parent
                         source: room.roomImage
                   }
          }

          LuxDomoLabel
          {
              id:name
              objectName: "name"
              width: room.width
              height:  room.height-room.width
              anchors.bottom: room.bottom
              textName: room.roomName
              textSize: nameSize
              textElide: true
          }

          MouseArea
          {
                 id:mouseArea
                 objectName: "mouseArea"
                 anchors.fill: parent
                 onPressAndHold: {   if(room.isMoveParent==false)  {  pLuxDomoRoomItem.showFunctionOption()   }  }
                 onClicked:  {  if(room.isMoveParent==false)  {  pLuxDomoRoomItem.roomManage()  } }
                 onPressed: {    pLuxDomoRoomItem.recordMouseY(mouseY) ;   room.oldY=mouseY  }
                 onReleased:   {  room.isMoveParent=false  }
                 onPositionChanged:
                 {
                              if(mouseY-oldY>5||oldY-mouseY>5)
                              {
                                    pLuxDomoRoomItem.moveParent(mouseY);
                                   room.isMoveParent=true
                              }
                 }
          }
}

