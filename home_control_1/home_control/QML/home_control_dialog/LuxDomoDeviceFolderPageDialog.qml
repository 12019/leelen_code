import  QtQuick 1.0
import  LuxDomoDeviceFolderPage   1.0
Rectangle
{
    id:deviceFolderPage
    objectName: "deviceFolderPage"
     width: 800
     height: 480
     color: "#00000000"
     property int    showAreaW: 0
     property int    showAreaH: 0
     property int    showAreaX: 0
     property int    showAreaY: 0

     function    setDeviceControl(objectName,location) {  pLuxDomoDeviceFolderPage.setDeviceControlLocation(objectName,location) }
     function    removeAlldevice(){  pLuxDomoDeviceFolderPage.removeAllDevice()  }
     function    deleteDeviceControl(objectName){  pLuxDomoDeviceFolderPage.deviceMoveOutFolder(objectName)   }
     function    on_offDevice(deviceUid,value){  pLuxDomoDeviceFolderPage.on_offDevice(deviceUid,value)  }

     LuxDomoDeviceFolderPageItem
     {
          id:pLuxDomoDeviceFolderPage
          objectName: "pLuxDomoDeviceFolderPage"
     }
     onYChanged: {   deviceFolderPage.y=0   }

     MouseArea
     {
         id:mouseArea
         objectName: "mouseArea"
         drag.target: deviceFolderPage
         drag.axis: Drag.XandYAxis
         anchors.fill: parent
         onClicked:
         {
             if(mouseX<showAreaX||mouseY<showAreaY||mouseX>(showAreaX+showAreaW)||mouseY>(showAreaY+showAreaH))
             {  pLuxDomoDeviceFolderPage.closeFolderPage() }
          }
         onReleased:
         {
             if(0>deviceFolderPage.x)
              {
                 if((-deviceFolderPage.x)>100)  {  pLuxDomoDeviceFolderPage.folderPageMove("left")    }
              }
             else
              {
                 if(deviceFolderPage.x>100)   {   pLuxDomoDeviceFolderPage.folderPageMove("right")   }
               }
             deviceFolderPage.x=0
             deviceFolderPage.y=0
          }
    }

     MouseArea
     {
         id:changName
         width: parent.width/5
         height: showAreaY
         anchors.horizontalCenter: parent.horizontalCenter
         y:0
         onClicked: {   pLuxDomoDeviceFolderPage.changFolderName()  }
     }
}

