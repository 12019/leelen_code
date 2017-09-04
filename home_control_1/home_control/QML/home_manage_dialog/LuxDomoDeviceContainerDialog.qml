import  QtQuick 1.0
import  LuxDomoDeviceContainer  1.0
import   "../content"
Rectangle
{
           id:deviceContaniner
           objectName: "deviceContaniner"
           width: 800
           height: 480
           color: "#00000000"

           property int   addDeviceX: deviceContaniner.width
           property int   addDeviceY:deviceContaniner.height
           property int   addDeviceW: 80
           property int   addDeviceH: 110
           property string  roomObjectName: ""
           function   init(){   pLuxDomoDeviceContainerItem.init()  }
           function   deleteDevice(deviceUid){   pLuxDomoDeviceContainerItem.deleteDevice(deviceUid) }
           function   addDevice(type,address,attribute,name){   pLuxDomoDeviceContainerItem.addDevice(type,address,attribute,name)  }
           function   setDeviceInforMation(data){   pLuxDomoDeviceContainerItem.setDeviceInforMation(data) }
           function   setOperateDevice(deviceUid){   pLuxDomoDeviceContainerItem.setOperateDevice(deviceUid)   }
           property   int  screenH: 480
           property   int  nameSize: 20

           LuxDomoDeviceContainerItem
           {
               id:pLuxDomoDeviceContainerItem
               objectName: "pLuxDomoDeviceContainerItem"
           }

           onYChanged:
           {
                   if(deviceContaniner.y>0)  {  deviceContaniner.y=0   }
                   else if(-deviceContaniner.y>deviceContaniner.height-screenH)  {  deviceContaniner.y=screenH-deviceContaniner.height   }
           }

           MouseArea
           {
                  id:mouseArea
                  objectName: "mouseArea"
                  anchors.fill: parent
                  drag.target: deviceContaniner
                  drag.axis: Drag.YAxis
           }

           Rectangle
           {
                 id:addDevice
                 objectName: "addDevice"
                 height: addDeviceH
                 width: addDeviceW
                 x:deviceContaniner.addDeviceX
                 y:deviceContaniner.addDeviceY
                 color: "#00000000"

                 LuxDomoButton
                 {
                        id:addButton
                        height: addDevice.width
                        width: addDevice.width
                        anchors.top: addDevice.top
                        anchors.left: addDevice.left
                        image: "../home_manage/add_on.png"
                        imagepressed: "../home_manage/add_on.png"
                        buttonColor: "#00000000"
                        onClicked: {   pLuxDomoDeviceContainerItem.showDeviceOption()  }
                  }

                 LuxDomoLabel
                 {
                     id:textRec
                     objectName: "textRec"
                     width: addDevice.width
                     height:  addDevice.height-addDevice.width
                     anchors.bottom: addDevice.bottom
                     textName: "添加设备"
                     textSize: nameSize
                 }
           }
}

