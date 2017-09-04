import   QtQuick 1.0
import   LuxDomoSceneSetting  1.0
import   "../content"
Rectangle
{
    id:sceneSetting
    objectName: "sceneSetting"
    width: 450
    height: 480
    color:"#efeff4"

    property string sceneName: "ggg"
    property  string    sceneCondition: ""

    function    clearModel()      {   model.clear()    }
    function    addModel(room_objectName,device_objectName,device_name,device_power,device_type,device_actflag)
                        {
                             model.append({"room_objectName":room_objectName,"device_objectName":device_objectName,"device_name":device_name,
                                                        "device_power":device_power,"device_type":device_type,"device_actflag":device_actflag})
                         }

    function     init(strFile,name){  pLuxDomoSceneSettingItem.init(strFile,name)  }
    function     changModel(index,propertyName,propertyValue){  model.setProperty( index,propertyName,propertyValue) }

    MouseArea{   anchors.fill: parent    }

    LuxDomoSceneSettingItem
    {
          id:pLuxDomoSceneSettingItem
          objectName: "pLuxDomoSceneSettingItem"
    }

    Rectangle
    {
        id:titleBar
        width: parent.width
        height: parent.height/8
        anchors.top: parent.top
        color:"#f6f6f7"
        Rectangle
        {
           width: parent.width
           height: 1
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
                LuxDomoButton
                {
                      width: parent.width/2
                      height: parent.height*2/3
                      anchors.verticalCenter: parent.verticalCenter
                      anchors.horizontalCenter: parent.horizontalCenter
                      name:"取消"
                      textcolor: "#24c3be"
                      fontsize: 22
                      showImage: false
                      onClicked: {   pLuxDomoSceneSettingItem.close()  }
                }
            }
            Item
            {
                width: parent.width/2
                height: parent.height
                LuxDomoLabel
                {
                   anchors.fill: parent
                   textName: sceneName
                   textSize: 27
                }
            }
            Item
            {
                width: parent.width/4
                height: parent.height
                LuxDomoButton
                {
                    width: parent.width/2
                    height: parent.height*2/3
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    name:"保存"
                    textcolor: "#24c3be"
                    showImage: false
                    fontsize: 22
                    onClicked:  {   pLuxDomoSceneSettingItem.changSceneData("NULL","Save","Save","NULL")    }
                }
            }
        }
    }

    Item
    {
        id:controlButton
        width: parent.width
        height: parent.height/6
        anchors.top: titleBar.bottom
        Row
        {
            anchors.fill: parent
            Item
            {
                width: parent.width/5
                height: parent.height
                Image
                {
                    anchors.fill: parent
                    source: sceneCondition=="allOn"?"../setting/allControl_on.png":"../setting/allControl_off.png"
                }
                Column
                {
                     width: parent.width*3/5
                     height: parent.height*5/6
                     anchors.bottom: parent.bottom
                     anchors.horizontalCenter: parent.horizontalCenter
                     Item
                     {
                         width: parent.width*2/3
                         height: parent.width*2/3
                         anchors.horizontalCenter: parent.horizontalCenter
                         LuxDomoButton
                         {
                             anchors.fill: parent
                             image:"../setting/allOn.png"
                             imagepressed: "../setting/allOn.png"
                             onClicked:
                             {
                                 sceneCondition="allOn"
                                 pLuxDomoSceneSettingItem.changSceneData("power","AKeyOperate","AKeyOperate","on")
                             }
                         }
                     }
                     Item
                     {
                         width: parent.width
                         height: parent.height-parent.width*2/3
                         anchors.horizontalCenter: parent.horizontalCenter
                         LuxDomoLabel
                         {
                             anchors.fill: parent
                             textName: "全开"
                             textSize: 20
                         }
                     }
                }
            }
            Item
            {
                width: parent.width/5
                height: parent.height
                Image
                {
                    anchors.fill: parent
                    source: sceneCondition=="allOff"?"../setting/allControl_on.png":"../setting/allControl_off.png"
                }
                Column
                {
                     width: parent.width*3/5
                     height: parent.height*5/6
                     anchors.bottom: parent.bottom
                     anchors.horizontalCenter: parent.horizontalCenter
                     Item
                     {
                         width: parent.width*2/3
                         height: parent.width*2/3
                         anchors.horizontalCenter: parent.horizontalCenter
                         LuxDomoButton
                         {
                             anchors.fill: parent
                             image:"../setting/allOff.png"
                             imagepressed:"../setting/allOff.png"
                             onClicked:
                             {
                                 sceneCondition="allOff"
                                 pLuxDomoSceneSettingItem.changSceneData("power","AKeyOperate","AKeyOperate","off")
                             }
                         }
                     }
                     Item
                     {
                         width: parent.width
                         height: parent.height-parent.width*2/3
                         anchors.horizontalCenter: parent.horizontalCenter
                         LuxDomoLabel
                         {
                             anchors.fill: parent
                             textName: "全关"
                             textSize: 20
                         }
                     }
                }
            }
            Item
            {
                width: parent.width/5
                height: parent.height
                Image
                {
                    anchors.fill: parent
                    source: sceneCondition=="allActivate"?"../setting/allControl_on.png":"../setting/allControl_off.png"
                }
                Column
                {
                     width: parent.width*3/5
                     height: parent.height*5/6
                     anchors.bottom: parent.bottom
                     anchors.horizontalCenter: parent.horizontalCenter
                     Item
                     {
                         width: parent.width*2/3
                         height: parent.width*2/3
                         anchors.horizontalCenter: parent.horizontalCenter
                         LuxDomoButton
                         {
                             anchors.fill: parent
                             image:"../setting/allEnable.png"
                             imagepressed:"../setting/allEnable.png"
                             onClicked:
                             {
                                 sceneCondition="allActivate"
                                  pLuxDomoSceneSettingItem.changSceneData("actflag","AKeyOperate","AKeyOperate","enable")
                             }
                         }
                     }
                     Item
                     {
                         width: parent.width
                         height: parent.height-parent.width*2/3
                         anchors.horizontalCenter: parent.horizontalCenter
                         LuxDomoLabel
                         {
                             anchors.fill: parent
                             textName: "一键激活"
                             textSize: 20
                         }
                     }
                }
            }
            Item
            {
                width: parent.width/5
                height: parent.height
                Image
                {
                    anchors.fill: parent
                    source: sceneCondition=="allRelieve"?"../setting/allControl_on.png":"../setting/allControl_off.png"
                }
                Column
                {
                     width: parent.width*3/5
                     height: parent.height*5/6
                     anchors.bottom: parent.bottom
                     anchors.horizontalCenter: parent.horizontalCenter
                     Item
                     {
                         width: parent.width*2/3
                         height: parent.width*2/3
                         anchors.horizontalCenter: parent.horizontalCenter
                         LuxDomoButton
                         {
                             anchors.fill: parent
                             image:"../setting/allDisenable.png"
                             imagepressed:"../setting/allDisenable.png"
                             onClicked:
                             {
                                 sceneCondition="allRelieve"
                                 pLuxDomoSceneSettingItem.changSceneData("actflag","AKeyOperate","AKeyOperate","disenable")
                             }
                         }
                     }
                     Item
                     {
                         width: parent.width
                         height: parent.height-parent.width*2/3
                         anchors.horizontalCenter: parent.horizontalCenter
                         LuxDomoLabel
                         {
                             anchors.fill: parent
                             textName: "一键解除"
                             textSize: 20
                         }
                     }
                }
            }
            Item
            {
                width: parent.width/5
                height: parent.height
                Image
                {
                    anchors.fill: parent
                    source: sceneCondition=="memory"?"../setting/allControl_on.png":"../setting/allControl_off.png"
                }
                Column
                {
                     width: parent.width*3/5
                     height: parent.height*5/6
                     anchors.bottom: parent.bottom
                     anchors.horizontalCenter: parent.horizontalCenter
                     Item
                     {
                         width: parent.width*2/3
                         height: parent.width*2/3
                         anchors.horizontalCenter: parent.horizontalCenter
                         LuxDomoButton
                         {
                             anchors.fill: parent
                             image:"../setting/memory.png"
                             imagepressed:"../setting/memory.png"
                             onClicked:
                             {
                                 sceneCondition="memory"
                                 pLuxDomoSceneSettingItem.changSceneData("NULL","Memory","Memory","NULL")
                             }
                         }
                     }
                     Item
                     {
                         width: parent.width
                         height: parent.height-parent.width*2/3
                         anchors.horizontalCenter: parent.horizontalCenter
                         LuxDomoLabel
                         {
                             anchors.fill: parent
                             textName: "记忆"
                             textSize: 20
                         }
                     }
                }
            }
        }
    }

    ListModel
    {
         id: model
         ListElement { room_objectName:"999";device_objectName:"999";
                                device_name:" 00000" ;device_power:"off";
                                device_type:"var"; device_actflag:"enable"                }
     }

    Component
     {
           id:delegate
           Item
           {
                   id:device
                   objectName: "device"
                   width: list.width
                   height: list.height/5
                   Rectangle
                   {
                       width: parent.width
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
                           width: parent.width/3
                           height: parent.height
                           LuxDomoLabel
                           {
                               anchors.fill: parent
                               textName:device_name
                               textSize: 20
                               textElide: true
                               totalLength: 8
                               showLength: 6
                           }
                           MouseArea
                           {
                               anchors.fill: parent
                               onClicked:  { pLuxDomoSceneSettingItem.editDevice(device_objectName,device_type) }
                           }
                       }
                       Item
                       {
                           width: parent.width/3
                           height: parent.height
                           LuxDomoButton
                           {
                               width: parent.height
                               height: parent.height/2
                               anchors.verticalCenter: parent.verticalCenter
                               anchors.horizontalCenter: parent.horizontalCenter
                               image:device_power=="on"?"../setting/on_off_on.png":"../setting/on_off_off.png"
                                imagepressed:device_power=="on"?"../setting/on_off_on.png":"../setting/on_off_off.png"
                               onClicked:
                               {
                                       if(device_power=="off")
                                       {
                                              pLuxDomoSceneSettingItem.changSceneData("power",room_objectName,device_objectName,"on",index)
                                       }
                                       else
                                       {
                                              pLuxDomoSceneSettingItem.changSceneData("power",room_objectName,device_objectName,"off",index)
                                       }
                               }
                           }
                       }
                       Item
                       {
                           width: parent.width/3
                           height: parent.height
                           LuxDomoButton
                           {
                               width: parent.height/2
                               height: parent.height/2
                               anchors.verticalCenter: parent.verticalCenter
                               anchors.horizontalCenter: parent.horizontalCenter
                               image:device_actflag=="enable"?"../setting/enable.png":"../setting/disenable.png"
                               imagepressed:device_actflag=="enable"?"../setting/enable.png":"../setting/disenable.png"
                               onClicked:
                               {
                                   if(device_actflag=="enable")
                                   {
                                       pLuxDomoSceneSettingItem.changSceneData("actflag",room_objectName,device_objectName,"disenable",index)
                                   }
                                   else
                                   {
                                        pLuxDomoSceneSettingItem.changSceneData("actflag",room_objectName,device_objectName,"enable",index)
                                   }
                               }
                           }
                       }
                   }
          }
     }

    ListView
    {
        id: list
        width: parent.width*9/10
        height: parent.height*2/3
        y:parent.height/3
        x:parent.width/20
        clip: true
        model: model
        delegate: delegate
    }
}

