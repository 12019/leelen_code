import  QtQuick 1.0
import  LuxDomoRoomDevice 1.0
import  "../content"
Rectangle
{
        id:roomDevice
        objectName: "roomDevice"
        width: 80
        height: 100
        color: "#00000000"

        property  string    deviceName: ""
        property  string    deviceAddress: ""
        property  string    deviceAttribute: ""
        property  string    deviceType: ""
        property  string    on_off: ""
        property  string    deviceImage:""
        property  string    roomObjectName: ""
        property  int         nameSize: 20

        property  int  screenW: 800
        property  int  screenH:  480
        property  string inwhich: ""
        property  bool   isMove: false
        property  bool   isRuning: false
        property  bool   nameShow: true
        property  bool   isMoveParent: false
        property  int      oldX: 0

        LuxDomoRoomDeviceItem
        {
              id:pLuxDomoRoomDevice
              objectName: "pLuxDomoRoomDevice"
        }

       Rectangle
       {
              id:imagerec
              objectName: "imagerec"
              width: roomDevice.width
              height: roomDevice.width
              anchors.top: roomDevice.top
              anchors.left: roomDevice.left
              color: "#00000000"
              Image
                 {
                       anchors.fill: parent
                       source: roomDevice.deviceImage
                 }
        }

       LuxDomoLabel
       {
           id:textEdit
           objectName: "textEdit"
           width: roomDevice.width
           height:  roomDevice.height-roomDevice.width
           anchors.bottom: roomDevice.bottom
           visible: roomDevice.nameShow
           textName:roomDevice.deviceName
           textSize: nameSize
           textElide: true
       }

        Timer
        {
              id:time
              objectName: "time"
              interval: 1000
              repeat: false
              triggeredOnStart: false
              onTriggered:
                {
                         pLuxDomoRoomDevice.isChangPage(roomDevice.x)
                        time.stop()
                        roomDevice.isRuning=false
               }
         }

        onYChanged:
        {
            if(roomDevice.isMove)
            {
                      if(roomDevice.y<roomDevice.screenH/8)
                                {  roomDevice.y=roomDevice.screenH/8 }
                     else if(roomDevice.y>(roomDevice.screenH-roomDevice.height))
                                       {  roomDevice.y=roomDevice.screenH-roomDevice.height  }
            }
         }

        onXChanged:
        {
                if(roomDevice.isMove)
                {
                     if(roomDevice.x<0||roomDevice.x>(roomDevice.screenW-roomDevice.width))
                      {    if(roomDevice.isRuning==false)    {   time.start();    roomDevice.isRuning=true;   }   }
                      else
                      {    if(roomDevice.isRuning==true)     {    time.stop();    roomDevice.isRuning=false   }   }
                }
        }

        MouseArea
        {
                  id:mouseArea
                  objectName: "mouseArea"
                  anchors.fill: parent
                  drag.target:  roomDevice.isMove?roomDevice:Drag.active
                  drag.axis: Drag.XandYAxis
                  enabled: roomDevice.nameShow
                  onClicked:
                  {
                            if(roomDevice.isMoveParent==false)
                            {
                                    pLuxDomoRoomDevice.controlDevice()
                            }
                  }
                  onPressAndHold:
                  {
                           if(roomDevice.isMoveParent==false)
                           {
                               roomDevice.isMove=true ;
                               roomDevice.width+=15;
                               textEdit.textName=""
                               pLuxDomoRoomDevice.rememberXY()
                           }
                  }
                  onPressed:  {  pLuxDomoRoomDevice.recordMouseX(mouseX) ;  roomDevice.oldX=mouseX }
                  onReleased:
                  {
                             if(roomDevice.isMove)
                             {
                                       roomDevice.isMove=false;   roomDevice.width-=15;    textEdit.textName=roomDevice.deviceName
                                       if(roomDevice.inwhich=="room") {    pLuxDomoRoomDevice.moveStop(roomDevice.x,roomDevice.y)   }
                                       else  if(roomDevice.inwhich=="folder")  {  pLuxDomoRoomDevice.otherMoveStop()  }
                              }
                             else
                              {         roomDevice.isMoveParent=false
                                       pLuxDomoRoomDevice.changPage()
                              }
                   }
                  onPositionChanged:
                  {
                              if(roomDevice.isMove)
                              {
                                       if(roomDevice.inwhich=="page") {    pLuxDomoRoomDevice.moveStart()     }
                                       else   if(roomDevice.inwhich=="folder")
                                                    {     pLuxDomoRoomDevice.otherMoveStart(roomDevice.x,roomDevice.y)  }
                               }
                              else  if(roomDevice.oldX-mouseX>5||mouseX-roomDevice.oldX>5)
                               {
                                        pLuxDomoRoomDevice.moveParent(mouseX)
                                        roomDevice.isMoveParent=true
                               }
                    }
        }

}

