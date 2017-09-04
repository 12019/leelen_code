import   QtQuick 1.0
import   LuxDomoHomeManage  1.0

Rectangle
{
           id:homeManage
           objectName: "homeManage"
           width: 800
           height: 480
           color: "#e47878"

           MouseArea
           {
                 id:mouseArea
                 objectName: "mouseArea"
                 anchors.fill: homeManage
                 onClicked: {  console.log("homeManagehomeManagehomeManagehomeManagehomeManagehomeManage")  }
           }

         BorderImage
         {
             id: image
             objectName: "image"
             width: parent.width
             height: parent.height*7/8
             anchors.bottom: parent.bottom
             source: "../home_manage/room_background.png"
         }

           LuxDomoHomeManageItem
           {
                 id:pLuxDomoHomeManageItem
                 objectName: "pLuxDomoHomeManageItem"
           }
}

