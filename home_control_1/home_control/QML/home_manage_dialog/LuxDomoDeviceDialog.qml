import   QtQuick 1.0
import   LuxDomoDevice  1.0
import   "../content"
Rectangle
{
          id:device
          objectName: "device"
          width: 80
          height: 100
          color: "#00000000"

          property  string   roomObjectName: ""
          property  string   deviceName: ""
          property  string   deviceAddress: ""
          property  string   deviceAttribute: ""
          property  string   deviceType: ""
          property  string   on_off: ""
          property  string   deviceImage: ""
          property  int         nameSize: 20

          property  bool     isMoveParent: false
          property  int        oldY: 0

         LuxDomoDeviceItem
        {
               id:pLuxDomoDeviceItem
              objectName: "pLuxDomoDeviceItem"
        }

       Rectangle
       {
             id:image
             objectName: "image"
             width: device.width
             height: device.width
             anchors.top: device.top
             anchors.left: device.left
             color: "#00000000"
             Image
             {
                   anchors.fill: parent
                   source: device.deviceImage
              }
       }

       LuxDomoLabel
       {
           id:name
           objectName: "name"
           width: device.width
           height:  device.height-device.width
           anchors.bottom: device.bottom
           textName: device.deviceName
           textSize: nameSize
           textElide: true
       }

      MouseArea
      {
             id:mouseArea
             objectName: "mouseArea"
             anchors.fill: parent
             onPressAndHold: {   if(device.isMoveParent==false)  pLuxDomoDeviceItem.showFunctionOption()   }
             onClicked:  {   if(device.isMoveParent==false)   pLuxDomoDeviceItem.showControlDialog() ;  }
             onPressed: {   pLuxDomoDeviceItem.recordMouseY(mouseY) ;  device.oldY=mouseY  }
             onReleased: {   device.isMoveParent=false   }
             onPositionChanged:
             {
                 if(device.oldY-mouseY>5||mouseY-device.oldY>5)
                 {
                          pLuxDomoDeviceItem.moveParent(mouseY)
                          device.isMoveParent=true
                 }
             }
      }
}

