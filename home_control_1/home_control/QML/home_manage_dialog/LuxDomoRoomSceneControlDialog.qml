import QtQuick 1.0
import  LuxDomoRoomSceneControl 1.0
import  "../content"
Rectangle
{
    id:sceneControl
    objectName: "sceneControl"
    width: 800
    height: 480
    color:"#efeff4"

    property  int         titleBarHeiht: 50
    property  string    sceneCondition: ""
    property  string    sceneName: "房间场景"

    function    clearModel()      {   model.clear()    }
    function    addModel(device_objectName,device_name,device_open,device_actflag,device_type)
                        {
                             model.append({"device_objectName":device_objectName,"device_name":device_name,"device_power":device_open,"device_actflag":device_actflag,"device_type":device_type})
                         }
    function     changModel(index,propertyName,propertyValue){  model.setProperty( index,propertyName,propertyValue) }
    function    init(roomObjectName,sceneObjectName){  pLuxDomoRoomSceneControlItem.init(roomObjectName,sceneObjectName)}

   LuxDomoRoomSceneControlItem
   {
          id:pLuxDomoRoomSceneControlItem
          objectName: "pLuxDomoRoomSceneControlItem"
    }

    MouseArea{  anchors.fill: parent  }

    Rectangle
    {
        id:titleBar
        objectName: "titleBar"
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
                      onClicked: {  pLuxDomoRoomSceneControlItem.close()   }
                }
            }
            Item
            {
                width: parent.width/2
                height: parent.height*2/3
                LuxDomoLabel
                {
                   anchors.fill: parent
                   textName: sceneControl.sceneName
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
                     onClicked:  {   pLuxDomoRoomSceneControlItem.changSceneData("NULL","Save","NULL")   }
                }
            }
        }
    }

    ListModel
    {
            id: model
            ListElement { device_objectName:"999";  device_name:" 00000" ;device_power:"off"; device_actflag:"enable" ;device_type:"ac"}
    }

    Component
     {
           id:delegate
           Item
           {
                   id:device
                   objectName: "device"
                   width: list.width
                   height: list.height/6
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
                               totalLength: 12
                               showLength: 10
                           }
                           MouseArea
                           {
                               anchors.fill: parent
                               onClicked:  {  pLuxDomoRoomSceneControlItem.editDevice(device_objectName,device_type)  }
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
                               image:device_power=="on"?"../home_manage/on_off_on.png":"../home_manage/on_off_off.png"
                               imagepressed: device_power=="on"?"../home_manage/on_off_on.png":"../home_manage/on_off_off.png"
                               onClicked:
                               {
                                       if(device_power=="off")
                                       {
                                           pLuxDomoRoomSceneControlItem.changSceneData("power",device_objectName,"on",index)
                                       }
                                       else
                                       {
                                           pLuxDomoRoomSceneControlItem.changSceneData("power",device_objectName,"off",index)
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
                               image:device_actflag=="enable"?"../home_manage/enable.png":"../home_manage/disenable.png"
                               imagepressed: device_actflag=="enable"?"../home_manage/enable.png":"../home_manage/disenable.png"
                               onClicked:
                               {
                                   if(device_actflag=="enable")
                                   {
                                              pLuxDomoRoomSceneControlItem.changSceneData("actflag",device_objectName,"disenable",index)
                                     }
                                    else
                                     {
                                             pLuxDomoRoomSceneControlItem.changSceneData("actflag",device_objectName,"enable",index)
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
            width: parent.width
            height:parent.height*2/3
            y:parent.height/6
            clip: true
            model: model
            delegate: delegate
    }

    Item
    {
        id:operateButtons
        objectName: "operateButtons"
        width: parent.width
        height:parent.height/6
        anchors.bottom: parent.bottom
        Rectangle
        {
            width: parent.width
            height: 1
            anchors.right:parent.right
            anchors.top:parent.top
            color: "#bfbfbf"
        }
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
                   source: sceneCondition=="allOn"?"../home_manage/allControl_on.png":"../home_manage/allControl_off.png"
               }
               Column
               {
                    width: parent.width*2/5
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
                            image:"../home_manage/allOn.png"
                            imagepressed: "../home_manage/allOn.png"
                            onClicked:
                            {
                                sceneControl.sceneCondition="allOn"
                                pLuxDomoRoomSceneControlItem.changSceneData("power","AKeyOperate","on")
                            }
                        }
                    }
                    Item
                    {
                        width: parent.width*2/3
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
                    source: sceneCondition=="allOff"?"../home_manage/allControl_on.png":"../home_manage/allControl_off.png"
                }
                Column
                {
                     width: parent.width*2/5
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
                             image: "../home_manage/allOff.png"
                             imagepressed: "../home_manage/allOff.png"
                             onClicked:
                             {
                                   sceneControl.sceneCondition="allOff"
                                  pLuxDomoRoomSceneControlItem.changSceneData("power","AKeyOperate","off")
                             }
                         }
                     }
                     Item
                     {
                         width: parent.width*2/3
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
                    source: sceneCondition=="allActivate"?"../home_manage/allControl_on.png":"../home_manage/allControl_off.png"
                }
                Column
                {
                     width: parent.width*2/5
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
                             image:"../home_manage/allEnable.png"
                             imagepressed: "../home_manage/allEnable.png"
                             onClicked:
                             {
                                 sceneControl.sceneCondition="allActivate"
                                 pLuxDomoRoomSceneControlItem.changSceneData("actflag","AKeyOperate","enable")
                             }
                         }
                     }
                     Item
                     {
                         width: parent.width*2/3
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
                    source: sceneCondition=="allRelieve"?"../home_manage/allControl_on.png":"../home_manage/allControl_off.png"
                }
                Column
                {
                     width: parent.width*2/5
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
                             image:"../home_manage/allDisenable.png"
                             imagepressed:"../home_manage/allDisenable.png"
                             onClicked:
                             {
                                 sceneControl.sceneCondition="allRelieve"
                                 pLuxDomoRoomSceneControlItem.changSceneData("actflag","AKeyOperate","disenable")
                             }
                         }
                     }
                     Item
                     {
                         width: parent.width*2/3
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
                    source: sceneCondition=="memory"?"../home_manage/allControl_on.png":"../home_manage/allControl_off.png"
                }
                Column
                {
                     width: parent.width*2/5
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
                             image:"../home_manage/memory.png"
                             imagepressed:"../home_manage/memory.png"
                             onClicked:
                             {
                                   sceneControl.sceneCondition="memory"
                                  pLuxDomoRoomSceneControlItem.changSceneData("NULL","Memory","NULL")
                             }
                         }
                     }
                     Item
                     {
                         width: parent.width*2/3
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
}
