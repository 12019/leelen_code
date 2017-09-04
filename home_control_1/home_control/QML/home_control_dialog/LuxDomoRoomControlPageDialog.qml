import  QtQuick 1.0
import  LuxDomoRoomControlPage 1.0

Rectangle
{
          id:roomControlpage
          objectName: "roomControlpage"
          width: 600
          height: 480
          color: "#00000000"

          function   on_offDevice(deviceUid,value){  pLuxDomoRoomControlPage.on_offDevice(deviceUid,value)  }

          LuxDomoRoomControlPageItem
          {
               id:pLuxDomoRoomControlPage
               objectName: "pLuxDomoRoomControlPage"
          }
         onYChanged:  {    roomControlpage.y=0   }

          MouseArea
          {
                  id:mouseArea
                  objectName: "mouseArea"
                  anchors.fill: parent
                  drag.target: roomControlpage
                  drag.axis: Drag.XandYAxis
                  onReleased:
                  {
                                      if(0>roomControlpage.x)
                                      {       if((-roomControlpage.x)>150)
                                              {
                                                       pLuxDomoRoomControlPage.leftMove()
                                              }
                                       }
                                     else
                                       {     if(roomControlpage.x>150)
                                              {
                                                        pLuxDomoRoomControlPage.rightMove()
                                               }
                                       }
                                      roomControlpage.x=0
                                      roomControlpage.y=0
                    }
                    onClicked:
                    {
                                if(mouseY>=0&&mouseY<=roomControlpage.height/8)
                                {
                                        if(mouseX>=0&&mouseX<=roomControlpage.width/6)
                                        {    pLuxDomoRoomControlPage.back()     }
                                        else  if(mouseX>=roomControlpage*5/6&&mouseX<=roomControlpage.width)
                                                   {       }
                                 }

                      }
          }
}

