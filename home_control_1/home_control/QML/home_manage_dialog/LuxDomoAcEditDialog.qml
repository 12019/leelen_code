import   QtQuick 1.0
import   LuxDomoAcEdit 1.0
import   "../content"
Rectangle
 {
     id:acEdit
     objectName: "acEdit"
     width: 700
     height: 480
     color: "#00000000"

     property string  mode: ""
     property string  wind: ""
     property string  speed: ""
     property string  temp: ""

     property string  arrowsCondition: ""
     property int       pointX: 300

     function   init(type,deviceUid){   pLuxDomoAcEditItem.init(type,deviceUid)  }

     LuxDomoAcEditItem
     {
         id:pLuxDomoAcEditItem
         objectName: "pLuxDomoAcEditItem"
     }

     MouseArea
     {
         anchors.fill: parent
     }

     Item
     {
         width: parent.width/2
         height: parent.height*7/9
         anchors.verticalCenter: parent.verticalCenter
         x:pointX
         Image
         {
             anchors.fill: parent
             source: "../home_manage/deviceEdit_top.png"
         }
         Column
         {
             anchors.fill: parent
             Item
             {
                 width: parent.width
                 height: parent.height/7
                 Rectangle
                 {
                    width: parent.width*9/10
                    height: 1
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: "#bfbfbf"
                 }
                 LuxDomoLabel
                 {
                      anchors.fill: parent
                      textName: "空调设置"
                      textSize: 27
                      textColor: "#141512"
                 }
             }
             Item
             {
                 width: parent.width
                 height: parent.height/7
                 Row
                 {
                     width: parent.width
                     height: parent.height*4/5
                     anchors.left: parent.left
                     anchors.bottom: parent.bottom
                     Item
                     {
                           width: parent.width*2/5
                           height: parent.height
                           LuxDomoLabel
                           {
                                 anchors.fill: parent
                                 textName: "模式选择"
                                 textColor: "#676767"
                                 textSize: 20
                           }
                     }
                     Item
                     {
                         width: parent.width*3/5
                         height: parent.height
                         Image
                         {
                             width: parent.width*2/3
                             height: parent.height
                             source:arrowsCondition=="mode"?"../home_manage/option_on.png":"../home_manage/option_off.png"
                         }
                         LuxDomoStyleLabel
                         {
                               width: parent.width*2/3
                               height: parent.height
                               anchors.left: parent.left
                               anchors.top: parent.top
                               textName: acEdit.mode
                               textSize: 22
                               textColor: "#151412"
                               textImage:arrowsCondition=="mode"?"../home_manage/up_arrows.png":"../home_manage/down_arrows.png"
                               onClicked:
                               {
                                   arrowsCondition="mode"
                                   pLuxDomoAcEditItem.showModeList()
                               }
                         }
                     }
                 }
             }
             Item
             {
                 width: parent.width
                 height: parent.height/7
                 Row
                 {
                     width: parent.width
                     height: parent.height*4/5
                     anchors.left: parent.left
                     anchors.bottom: parent.bottom
                     Item
                     {
                           width: parent.width*2/5
                           height: parent.height
                           LuxDomoLabel
                           {
                                 anchors.fill: parent
                                 textName: "温度选择"
                                 textColor: "#676767"
                                 textSize: 20
                           }
                     }
                     Item
                     {
                         width: parent.width*3/5
                         height: parent.height
                         Image
                         {
                             width: parent.width*2/3
                             height: parent.height
                             source: arrowsCondition=="temp"?"../home_manage/option_on.png":"../home_manage/option_off.png"
                         }
                         LuxDomoStyleLabel
                         {
                               width: parent.width*2/3
                               height: parent.height
                               anchors.left: parent.left
                               anchors.top: parent.top
                               textName: acEdit.temp
                               textSize: 22
                               textColor: "#151412"
                               textImage:arrowsCondition=="temp"?"../home_manage/up_arrows.png":"../home_manage/down_arrows.png"
                               onClicked:
                               {
                                   arrowsCondition="temp"
                                   pLuxDomoAcEditItem.showTempList()
                               }
                         }
                     }
                 }
             }
             Item
             {
                 width: parent.width
                 height: parent.height/7
                 Row
                 {
                     width: parent.width
                     height: parent.height*4/5
                     anchors.left: parent.left
                     anchors.bottom: parent.bottom
                     Item
                     {
                           width: parent.width*2/5
                           height: parent.height
                           LuxDomoLabel
                           {
                                 anchors.fill: parent
                                 textName: "风速选择"
                                 textColor: "#676767"
                                 textSize: 20
                           }
                     }
                     Item
                     {
                         width: parent.width*3/5
                         height: parent.height
                         Image
                         {
                             width: parent.width*2/3
                             height: parent.height
                             source:arrowsCondition=="speed"?"../home_manage/option_on.png":"../home_manage/option_off.png"
                         }
                         LuxDomoStyleLabel
                         {
                               width: parent.width*2/3
                               height: parent.height
                               anchors.left: parent.left
                               anchors.top: parent.top
                               textName:  acEdit.speed
                               textSize: 22
                               textColor: "#151412"
                               textImage:arrowsCondition=="speed"?"../home_manage/up_arrows.png":"../home_manage/down_arrows.png"
                               onClicked:
                               {
                                   arrowsCondition="speed"
                                   pLuxDomoAcEditItem.showSpeedList()
                               }
                         }
                     }
                 }
             }
             Item
             {
                 width: parent.width
                 height: parent.height/7
                 Row
                 {
                     width: parent.width
                     height: parent.height*4/5
                     anchors.left: parent.left
                     anchors.bottom: parent.bottom
                     Item
                     {
                           width: parent.width*2/5
                           height: parent.height
                           LuxDomoLabel
                           {
                                 anchors.fill: parent
                                 textName: "风向选择"
                                 textColor: "#676767"
                                 textSize: 20
                           }
                     }
                     Item
                     {
                         width: parent.width*3/5
                         height: parent.height
                         Image
                         {
                             width: parent.width*2/3
                             height: parent.height
                             source:arrowsCondition=="wind"?"../home_manage/option_on.png":"../home_manage/option_off.png"
                         }
                         LuxDomoStyleLabel
                         {
                               width: parent.width*2/3
                               height: parent.height
                               anchors.left: parent.left
                               anchors.top: parent.top
                               textName:acEdit.wind
                               textSize: 22
                               textColor: "#151412"
                               textImage:arrowsCondition=="wind"?"../home_manage/up_arrows.png": "../home_manage/down_arrows.png"
                               onClicked:
                               {
                                   arrowsCondition="wind"
                                   pLuxDomoAcEditItem.showWindList()
                               }
                         }
                     }
                 }
             }
             Item
             {
                 width: parent.width
                 height: parent.height*2/7
                 Row
                 {
                     width: parent.width
                     height: parent.height/2
                     anchors.verticalCenter: parent.verticalCenter
                     anchors.horizontalCenter: parent.horizontalCenter
                     Item
                     {
                          width: parent.width/2
                          height: parent.height
                          LuxDomoButton
                          {
                                 height: parent.height
                                 width: parent.width/2
                                 anchors.verticalCenter: parent.verticalCenter
                                 anchors.horizontalCenter: parent.horizontalCenter
                                 name:"取消"
                                 fontsize:22
                                 textcolor: "#24c3be"
                                 onClicked: {  pLuxDomoAcEditItem.close()  }
                          }
                     }
                     Item
                     {
                         width: parent.width/2
                         height: parent.height
                         LuxDomoButton
                         {
                                height: parent.height
                                width: parent.width/2
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                name:"确定"
                                fontsize:22
                                textcolor: "#24c3be"
                                onClicked: {  pLuxDomoAcEditItem.save()  }
                         }
                     }
                 }
             }
         }
     }
}

