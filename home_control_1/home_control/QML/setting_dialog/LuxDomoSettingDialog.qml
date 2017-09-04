import   QtQuick 1.0
import   LuxDomoSetting  1.0
import   "../content"
Rectangle
{
    id:setting
    objectName: "setting"
    width: 700
    height: 480
    color:"#efeff4"

    property   int   pressIndex: -100
    function    clearModel()        {     settingModel.clear()    }
    function    addModel(settingType,settingImage,settingName) {
                              settingModel.append({ "settingType":settingType,
                                                                      "settingImage":settingImage,
                                                                      "settingName":settingName  } ) }

    LuxDomoSettingItem
    {
        id:pLuxDomoSettingItem
        objectName: "pLuxDomoSettingItem"
    }

    MouseArea{  anchors.fill: parent  }

    Rectangle
    {
        id:settingTitle
        width: parent.width*5/14
        height:parent.height/8
        color:"#f6f6f7"
        LuxDomoLabel
        {
            anchors.fill: parent
            textName: "设置"
            textSize:27
            color:"#00000000"
        }
        Rectangle
        {
           width: parent.width
           height: 1
           anchors.bottom: parent.bottom
           color: "#bfbfbf"
        }
        Rectangle
        {
            width: 2
            height: parent.height
            anchors.right: parent.right
            color: "#c7c7ca"
        }
    }

     Rectangle
     {
         id:settingItem
         width: parent.width*5/14
         height: parent.height*3/8
         anchors.top: settingTitle.bottom
         anchors.topMargin: parent.height/24
         Rectangle
         {
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
            color: "#bfbfbf"
         }
         Rectangle
         {
            width: parent.width
            height: 1
            anchors.top: parent.top
            color: "#bfbfbf"
         }

         Column
         {
             anchors.fill: parent
             Rectangle
             {
                 width: parent.width
                 height: parent.height/3
                 color: pressIndex==-1?"#24c3be":"white"
                 Rectangle
                 {
                     width: parent.width*3/4
                     height: 1
                     anchors.right:parent.right
                     anchors.bottom: parent.bottom
                     color: "#bfbfbf"
                 }
                 Row
                 {
                     anchors.fill: parent
                     Item
                     {
                         width: parent.width/4
                         height: parent.height
                         Image
                         {
                             width: parent.height*3/5
                             height: parent.height*3/5
                             anchors.verticalCenter: parent.verticalCenter
                             anchors.horizontalCenter: parent.horizontalCenter
                             source: "../setting/terminalSetting.png"
                         }
                     }
                     Item
                     {
                         width: parent.width/2
                         height: parent.height
                         LuxDomoLabel
                         {
                             anchors.fill: parent
                             textName: "远程设置"
                             textSize: 20
                         }
                     }
                     Item
                     {
                         width: parent.width/4
                         height: parent.height
                         Image
                         {
                             width: parent.height/3
                             height:  parent.height/3
                             anchors.verticalCenter: parent.verticalCenter
                             anchors.horizontalCenter: parent.horizontalCenter
                             source: "../setting/goto.png"
                         }
                     }
                 }
                 MouseArea
                 {
                     anchors.fill: parent
                     onClicked:
                     {
                         pressIndex=-1
                         pLuxDomoSettingItem.chooseSettingOptions("terminal")
                     }
                 }
             }
             Rectangle
             {
                 width: parent.width
                 height: parent.height/3
                 color: pressIndex==-2?"#24c3be":"white"
                 Rectangle
                 {
                     width: parent.width*3/4
                     height: 1
                     anchors.right:parent.right
                     anchors.bottom: parent.bottom
                     color: "#bfbfbf"
                 }
                 Row
                 {
                     anchors.fill: parent
                     Item
                     {
                         width: parent.width/4
                         height: parent.height
                         Image
                         {
                             width: parent.height*3/5
                             height: parent.height*3/5
                             anchors.verticalCenter: parent.verticalCenter
                             anchors.horizontalCenter: parent.horizontalCenter
                             source:"../setting/cloudSetting.png"
                         }
                     }
                     Item
                     {
                         width: parent.width/2
                         height: parent.height
                         LuxDomoLabel
                         {
                             anchors.fill: parent
                             textName: "云服务代理"
                             textSize: 20
                         }
                     }
                     Item
                     {
                         width: parent.width/4
                         height: parent.height
                         Image
                         {
                             width: parent.height/3
                             height:  parent.height/3
                             anchors.verticalCenter: parent.verticalCenter
                             anchors.horizontalCenter: parent.horizontalCenter
                             source: "../setting/goto.png"
                         }
                     }
                 }
                 MouseArea
                 {
                     anchors.fill: parent
                     onClicked:
                     {
                         pressIndex=-2
                         pLuxDomoSettingItem.chooseSettingOptions("cloud")
                     }
                 }
             }
             Rectangle
             {
                 width: parent.width
                 height: parent.height/3
                 color: pressIndex==-3?"#24c3be":"white"
                 Rectangle
                 {
                     width: parent.width*3/4
                     height: 1
                     anchors.right:parent.right
                     anchors.bottom: parent.bottom
                     color: "#bfbfbf"
                 }
                 Row
                 {
                     anchors.fill: parent
                     Item
                     {
                         width: parent.width/4
                         height: parent.height
                         Image
                         {
                             width: parent.height*3/5
                             height: parent.height*3/5
                             anchors.verticalCenter: parent.verticalCenter
                             anchors.horizontalCenter: parent.horizontalCenter
                            source: "../setting/sceneSetting.png"
                         }
                     }
                     Item
                     {
                         width: parent.width/2
                         height: parent.height
                         LuxDomoLabel
                         {
                             anchors.fill: parent
                             textName: "场景设置"
                             textSize: 20
                         }
                     }
                     Item
                     {
                         width: parent.width/4
                         height: parent.height
                         Image
                         {
                             width: parent.height/3
                             height:  parent.height/3
                             anchors.verticalCenter: parent.verticalCenter
                             anchors.horizontalCenter: parent.horizontalCenter
                             source: "../setting/goto.png"
                         }
                     }
                 }
                 MouseArea
                 {
                     anchors.fill: parent
                     onClicked:
                     {
                         pressIndex=-3
                         pLuxDomoSettingItem.chooseSettingOptions("scene")
                     }
                 }
             }
         }
     }

     ListModel
     {
         id: settingModel
         ListElement
         {
             settingType:""
             settingImage:""
             settingName:""
         }
     }

     Component
      {
          id:settingDelegate
          Rectangle
          {
              id:dataItem
              width: settingList.width
              height: settingList.height/2
              color: pressIndex==index?"#24c3be":"white"
              Rectangle
              {
                  width: parent.width*3/4
                  height: 1
                  anchors.right:parent.right
                  anchors.bottom: parent.bottom
                  color: "#bfbfbf"
              }
              Row
              {
                  anchors.fill: parent
                  Item
                  {
                      width: parent.width/4
                      height: parent.height
                      Image
                      {
                          width: parent.height*3/5
                          height: parent.height*3/5
                          anchors.verticalCenter: parent.verticalCenter
                          anchors.horizontalCenter: parent.horizontalCenter
                          source: settingImage
                      }
                  }
                  Item
                  {
                      width: parent.width/2
                      height: parent.height
                      LuxDomoLabel
                      {
                          anchors.fill: parent
                          textName: settingName
                          textSize: 20
                      }
                  }
                  Item
                  {
                      width: parent.width/4
                      height: parent.height
                      Image
                      {
                          width: parent.height/3
                          height:  parent.height/3
                          anchors.verticalCenter: parent.verticalCenter
                          anchors.horizontalCenter: parent.horizontalCenter
                          source: "../setting/goto.png"
                      }
                  }
              }
              MouseArea
              {
                  anchors.fill: parent
                  onClicked:
                  {
                      pressIndex=index
                      pLuxDomoSettingItem.chooseSettingOptions(settingType)
                  }
              }
          }
     }

     Rectangle
     {
         width: parent.width*5/14
         height:parent.height*2/8
         anchors.top: settingItem.bottom
         anchors.topMargin: parent.height/24

         ListView
         {
              id: settingList
             anchors.fill: parent
             clip: true
             model: settingModel
             delegate:settingDelegate
         }

         Rectangle
         {
            width: parent.width
            height: 1
            anchors.top: parent.top
            color: "#bfbfbf"
         }
         Rectangle
         {
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
            color: "#bfbfbf"
         }
      }

     Rectangle
     {
         width: 2
         height: parent.height*7/8
         x:parent.width*5/14-2
         y:parent.height/8
         color: "#9fa0a4"
     }

    Item
    {
        width: parent.width*5/14
        height: parent.height/7
        anchors.bottom: parent.bottom
        LuxDomoButton
        {
            width: parent.width*2/3
            height: parent.height*2/3
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            name: "退出"
            fontsize: 25
            onClicked: {   pLuxDomoSettingItem.back()  }
        }
    }
}

