import QtQuick 1.0
Item
{
    id:bypass
    objectName: "bypass"
    width: 300
    height: 200

    signal  sureClicked
    signal  cancelClicked
    property   string  message : ""

    MouseArea{  anchors.fill: parent   }

    Image
    {
        anchors.fill: parent
        source: "../security/bypass_hint.png"
    }

   Column
   {
       anchors.fill: parent
       Item
       {
           width: parent.width
           height: parent.height/3
           LuxDomoLabel1
           {
               width: parent.width*3/5
               height: parent.height
               anchors.right: parent.right
               anchors.verticalCenter: parent.verticalCenter
               textName: "探测器旁路"
               textSize: 25
           }
           Image
           {
               height: parent.height/2
               width: parent.height/2
               anchors.right: parent.right
               anchors.rightMargin: parent.width*3/5
               anchors.verticalCenter: parent.verticalCenter
               source:  "../security/bypass.png"
           }
       }
       Item
       {
           width: parent.width
           height: parent.height/3
           LuxDomoLabel
           {
               anchors.fill: parent
               textName: message
               textSize: 20
               textColor:"#676767"
           }
       }
       Item
       {
           width: parent.width
           height: parent.height/3
           Row
           {
               anchors.fill: parent
               Item
               {
                   width: parent.width/2
                   height: parent.height
                   LuxDomoButton
                   {
                       width: parent.width*2/3
                       height: parent.height*2/3
                       anchors.horizontalCenter: parent.horizontalCenter
                       anchors.verticalCenter: parent.verticalCenter
                       name:"取消"
                       onClicked: {   cancelClicked()   }
                   }
               }
               Item
               {
                   width: parent.width/2
                   height: parent.height
                   LuxDomoButton
                   {
                       width: parent.width*2/3
                       height: parent.height*2/3
                       anchors.horizontalCenter: parent.horizontalCenter
                       anchors.verticalCenter: parent.verticalCenter
                       name:"确定"
                       onClicked: {   sureClicked()   }
                   }
               }
           }
       }
   }
}

