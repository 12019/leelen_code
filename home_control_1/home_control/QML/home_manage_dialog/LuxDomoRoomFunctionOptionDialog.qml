import  QtQuick 1.0
import   LuxDomoRoomFunctionOption 1.0
import   "../content"
Rectangle
{
    id:functionOption
    objectName: "functionOption"
    width: 700
    height: 480
    color:"#af000000"

    property  string  editRoomName: ""
    property  bool    isShowhint: false
    property  string  roomName: ""

   LuxDomoRoomFunctionOptionItem
   {
         id:pLuxDomoRoomFunctionOptionItem
         objectName: "pLuxDomoRoomFunctionOptionItem"
   }

   MouseArea
   {
       anchors.fill: parent
       onClicked: {   pLuxDomoRoomFunctionOptionItem.close()   }
   }

   Item
   {
       width: parent.width/3
       height: parent.height*4/8
       anchors.verticalCenter: parent.verticalCenter
       anchors.horizontalCenter: parent.horizontalCenter
       enabled: isShowhint==false?true:false
       visible: isShowhint==false?true:false
       Image
       {
           width: parent.width
           height: parent.height
           source: "../home_manage/functionOption_bottom.png"
       }
       Column
       {
           anchors.fill: parent
           Item
           {
               width: parent.width
               height: parent.height/3
               Rectangle
               {
                  width: parent.width*9/10
                  height: 1
                  anchors.horizontalCenter: parent.horizontalCenter
                  anchors.bottom: parent.bottom
                  color: "#bfbfbf"
               }
               LuxDomoLabel
               {
                   anchors.fill: parent
                   textName: "功能选项"
                   textSize: 27
               }
           }
           Item
           {
               width: parent.width
               height: parent.height/3
               LuxDomoButton
               {
                   width: parent.width*3/5
                   height: parent.height/2
                   anchors.verticalCenter: parent.verticalCenter
                   anchors.horizontalCenter: parent.horizontalCenter
//                   image:"../option_off.png"
//                   imagepressed: "../option_off.png"
                   name:"删除房间"
                   fontsize: 20
                   onClicked: {   isShowhint=true   }
               }
           }
           Item
           {
               width: parent.width
               height: parent.height/3
               LuxDomoButton
               {
                   width: parent.width*3/5
                   height: parent.height/2
                   anchors.verticalCenter: parent.verticalCenter
                   anchors.horizontalCenter: parent.horizontalCenter
//                   image:"../option_off.png"
//                   imagepressed: "../option_off.png"
                   name:"编辑名称"
                   fontsize: 20
                   onClicked: {  pLuxDomoRoomFunctionOptionItem.setRoomName()  }
               }
           }
       }
   }

   LuxDomoHintHurdle
   {
       width: parent.width*2/5
       height: parent.height/3
       anchors.verticalCenter: parent.verticalCenter
       anchors.horizontalCenter: parent.horizontalCenter
       enabled: isShowhint
       visible: isShowhint
       onCancelClicked: {    pLuxDomoRoomFunctionOptionItem.close()  }
       onSureClicked: {      pLuxDomoRoomFunctionOptionItem.deleteRoom()  }
   }
}

