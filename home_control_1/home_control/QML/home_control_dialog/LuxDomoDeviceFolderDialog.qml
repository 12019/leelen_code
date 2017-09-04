import  QtQuick 1.0
import   LuxDomoDeviceFolder 1.0
import   "../content"
Rectangle
{
       id:deviceFodler
       objectName: "deviceFodler"
       width: 80
       height: 100
       color: deviceFodler.state?"#af000000":"#00000000"
       radius: 10

       property int locationX: -1
       property int locationY: -1
       property string folderName: ""
       property int screenW: 800
       property int screenH:  480
       property string inwhich: ""
       property bool isMove: false
       property bool state: false
       property bool isRuning: false
       property  int   nameSize: 20

      property int    showAreaW: 0
      property int    showAreaH: 0
      property int    showAreaX: 0
      property int    showAreaY: 0


       function     deleteDeviceControl(objectName)   {   pLuxDomoDeviceFolder.deviceMoveOutFolder(objectName) }
       function     addDevice(objectName,x,y){     }
       function     setDeviceControlLocaton(objectName) { pLuxDomoDeviceFolder.setDeviceControlLocation(objectName) }

       function     leftMove(pageName){ pLuxDomoDeviceFolder.leftMove(pageName)  }
       function     rightMove(pageName){  pLuxDomoDeviceFolder.rightMove(pageName)  }
       function     hideFolderPage(){   pLuxDomoDeviceFolder.hideFolderPage()  }

       function    createDeviceControl(objectName,name,type,id,on_off,deviceIndex){  pLuxDomoDeviceFolder.createDeviceControl(objectName,name,type,id,on_off,deviceIndex)}
       function    changAllDeviceLocation() {   pLuxDomoDeviceFolder.changAllDeviceLocation()   }
       function    changName(name){  pLuxDomoDeviceFolder.changName(name)  }

       LuxDomoDeviceFolderItem
       {
              id:pLuxDomoDeviceFolder
              objectName: "pLuxDomoDeviceFolder"
       }

       LuxDomoLabel
       {
           id:showName
           objectName: "showName"
           color: "#00000000"
           width: parent.width/5
           height: deviceFodler.showAreaY
           anchors.horizontalCenter: parent.horizontalCenter
           y:0
           visible: deviceFodler.state
           enabled: deviceFodler.state
           textName: folderName
           textSize: 25
           textColor: "#ffffff"
       }

      Rectangle
      {
          id:showArea
          objectName: "showArea"
          radius: 15
          color: "#00000000"
          width: deviceFodler.showAreaW
          height: deviceFodler.showAreaH
          anchors.horizontalCenter: parent.horizontalCenter
          y:deviceFodler.showAreaY
          visible: deviceFodler.state
          enabled: deviceFodler.state
          Image
          {
              anchors.fill: parent
              source: "../room_control/folder_background.png"
          }
     }

     Rectangle
     {
         id:imagerec
         objectName: "imagerec"
         width: deviceFodler.width
         height: deviceFodler.width
         anchors.top: deviceFodler.top
         anchors.left: deviceFodler.left
         color:"#af000000"
         visible: deviceFodler.state?false:true
         radius: 10
      }

     LuxDomoLabel
     {
         id:textEdit
         objectName: "textEdit"
         width: deviceFodler.width
         height:  deviceFodler.height-deviceFodler.width
         anchors.bottom: deviceFodler.bottom
         visible: deviceFodler.state?false:true
         textName:deviceFodler.folderName
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
                      pLuxDomoDeviceFolder.isChangPage(deviceFodler.x)
                     time.stop()
                    deviceFodler.isRuning=false
            }
      }

      onYChanged:
      {
             if(deviceFodler.isMove)
            {
                 if(deviceFodler.y<deviceFodler.screenH/8)
                           {  deviceFodler.y=deviceFodler.screenH/8 }
                 else if(deviceFodler.y>(deviceFodler.screenH-deviceFodler.height))
                                   {  deviceFodler.y=deviceFodler.screenH-deviceFodler.height  }
             }
      }

      onXChanged:
      {
              if(deviceFodler.isMove)
              {
                   if(deviceFodler.x<0||deviceFodler.x>(deviceFodler.screenW-deviceFodler.width))
                    {    if(deviceFodler.isRuning==false)    {   time.start();    deviceFodler.isRuning=true;   }   }
                    else
                    {    if(deviceFodler.isRuning==true)     {    time.stop();    deviceFodler.isRuning=false   }   }
              }
      }

      MouseArea
      {
             id:mouseArea
             objectName: "mouseArea"
             drag.target: deviceFodler.isMove?deviceFodler:Drag.active
             drag.axis: Drag.XandYAxis
             anchors.fill: parent
             onPressed:  {     }
             onPressAndHold:
             {
                      if(deviceFodler.state==false)
                      {
                               deviceFodler.isMove=true
                               deviceFodler.width+=15
                               textEdit.textName=""
                      }
             }
             onClicked: {    if(deviceFodler.state==false)  {    pLuxDomoDeviceFolder.showFolderPage()   }    }
             onReleased:
             {
                         if(deviceFodler.isMove)
                         {
                              deviceFodler.isMove=false;    deviceFodler.width-=15;   textEdit.textName=deviceFodler.folderName
                              if(deviceFodler.inwhich=="room")
                              {  pLuxDomoDeviceFolder.moveStop(deviceFodler.x,deviceFodler.y);  }
                        }
              }
             onPositionChanged:
             {
                      if(deviceFodler.isMove)  {   if(deviceFodler.inwhich!="room")    {   pLuxDomoDeviceFolder.moveStart();    }  }
             }
     }

}

