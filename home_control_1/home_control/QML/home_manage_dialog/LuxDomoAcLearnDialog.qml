import  QtQuick 1.0
import  LuxDomoAcLearn  1.0
import  "../content"
Rectangle
{
    id:acLearn
    objectName: "acLearn"
    width: 700
    height: 480
    color:"#efeff4"

    property int   pressIndex: -1
    property string   wind: "固定"
    property string   mode: "制冷"
    property string   speed:"自动"
    property string   temp: "18"

    property    string    arrowsCondition: ""
    property    bool      isShowhint: false
    property    bool      isShowIrHint: false
    property    int         pressCode: -1

    function addModel(code,status,wind,mode,speed,temp,isLearned)
    {
               acModel.append({
                               "code":code,
                               "status":status,
                               "wind":wind,
                               "mode":mode,
                               "speed":speed,
                               "temp":temp,
                               "isLearned":isLearned }  )
    }

    function       clearModel()   {   acModel.clear()     }
    function       removeModel(index) {  acModel.remove(index)  }
    function      init(deviceUid){    pLuxDomoAcLearnItem.init(deviceUid)  }
    function     changModel(propertyName,propertyValue){  acModel.setProperty( pressIndex,propertyName,propertyValue) }
    function     learnCode(){  pLuxDomoAcLearnItem.learnCode()  }

    LuxDomoAcLearnItem
    {
             id:pLuxDomoAcLearnItem
             objectName: "pLuxDomoAcLearnItem"
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
            }
            Item
            {
                width: parent.width/2
                height: parent.height
                LuxDomoLabel
                {
                    anchors.fill: parent
                    textName: "空调红外学习"
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
                    fontsize: 22
                    onClicked:  {   pLuxDomoAcLearnItem.close()    }
                }
            }
        }
    }

    ListModel
    {
             id: acModel
             ListElement {
                 code:40
                 status: "开"
                 wind: "固定"
                 mode:"制冷"
                 speed:"高速"
                 temp:"18度"
                 isLearned:false  }
    }

    Component
     {
             id:acDelegate
             Rectangle
             {
                      id:acOperate
                      objectName: "acOperate"
                      width: acList.width
                      height: acList.height/6
                      color: pressIndex==index?"#24c3be":"white"
                      Row
                      {
                              id:acRow
                              objectName: "acRow"
                              anchors.fill: parent
                              LuxDomoLabel
                              {
                                    width:acRow.width/6
                                    height: acRow.height
                                    textName:status
                              }
                              LuxDomoLabel
                              {
                                    width:acRow.width/6
                                    height: acRow.height
                                    textName:wind
                              }
                              LuxDomoLabel
                              {
                                    width:acRow.width/6
                                    height: acRow.height
                                    textName:mode
                              }
                              LuxDomoLabel
                              {
                                    width:acRow.width/6
                                    height: acRow.height
                                    textName:speed
                              }
                              LuxDomoLabel
                              {
                                    width:acRow.width/6
                                    height: acRow.height
                                    textName:temp
                              }
                            Item
                            {
                                     width:acRow.width/6
                                     height: acRow.height
                                     Image
                                     {
                                          width: parent.width
                                          height: parent.height/2
                                          anchors.verticalCenter: parent.verticalCenter
                                          source: isLearned?"../home_manage/learn_on.png":"../home_manage/learn_off.png"
                                     }
                                     LuxDomoLabel
                                     {
                                         anchors.fill: parent
                                         textName: isLearned?"已学习":"未学习"
                                     }
                            }
                      }

                      MouseArea
                      {
                                 anchors.fill: parent
                                 onClicked:
                                 {
                                     pLuxDomoAcLearnItem.learnClicked(code)
                                     acLearn.pressIndex=index
                                     isShowIrHint=true
                                 }
                                 onPressAndHold:
                                 {
                                     acLearn.pressCode=code
                                     acLearn.pressIndex=index
                                     isShowhint=true
                                 }
                     }
             }
     }

    ListView
    {
              id: acList
              width: acLearn.width
              height:acLearn.height*6/8
              y:parent.height/8
              clip: true
              model: acModel
              delegate: acDelegate
   }

    Item
    {
             id:optionAdd
             objectName: "functionName"
             width: parent.width
             height: parent.height/8
             anchors.top: acList.bottom
             Rectangle
             {
                width: parent.width
                height: 1
                anchors.top: parent.top
                color: "#bfbfbf"
             }
             Row
             {
                     id:functionName
                     objectName: "functionName"
                      anchors.fill: parent
                      LuxDomoStyleLabel
                      {
                                height: parent.height
                                width:parent.width/5
                                color: "#e8e8d7"
                                textName:wind
                                textSize: 22
                                textColor: "#151412"
                                textImage:arrowsCondition=="wind"?"../home_manage/up_arrows1.png":"../home_manage/down_arrows1.png"
                                onClicked:
                                {
                                    arrowsCondition="wind"
                                    pLuxDomoAcLearnItem.showWindList()
                                }
                      }
                      Rectangle
                      {
                          height: parent.height
                          width: 1
                          color: "white"
                      }

                      LuxDomoStyleLabel
                      {
                                height: parent.height
                                width:parent.width/5
                                color: "#e8e8d7"
                                textName:mode
                                textSize: 22
                                textColor: "#151412"
                                textImage:arrowsCondition=="mode"?"../home_manage/up_arrows1.png":"../home_manage/down_arrows1.png"
                                onClicked:
                                {
                                    arrowsCondition="mode"
                                   pLuxDomoAcLearnItem.showModeList()
                                }
                      }
                      Rectangle
                      {
                          height: parent.height
                          width: 1
                          color: "white"
                      }

                      LuxDomoStyleLabel
                      {
                                height: parent.height
                                width:parent.width/5
                                color: "#e8e8d7"
                                textName:speed
                                textSize: 22
                                textColor: "#151412"
                                textImage:arrowsCondition=="speed"?"../home_manage/up_arrows1.png":"../home_manage/down_arrows1.png"
                                onClicked:
                                {
                                    arrowsCondition="speed"
                                    pLuxDomoAcLearnItem.showSpeedList()
                                }
                      }
                      Rectangle
                      {
                          height: parent.height
                          width: 1
                          color: "white"
                      }

                      LuxDomoStyleLabel
                      {
                                height: parent.height
                                width:parent.width/5
                                color: "#e8e8d7"
                                textName:temp
                                textSize: 22
                                textColor: "#151412"
                                textImage:arrowsCondition=="temp"?"../home_manage/up_arrows1.png":"../home_manage/down_arrows1.png"
                                onClicked:
                                {
                                    arrowsCondition="temp"
                                    pLuxDomoAcLearnItem.showTempList()
                                }
                      }
                      Rectangle
                      {
                          height: parent.height
                          width: 1
                          color: "white"
                      }

                      LuxDomoButton
                      {
                          height: parent.height
                          width:parent.width/5
                          color: "#24c3be"
                          name:"添加"
                          textcolor: "#151412"
                          onClicked: {    pLuxDomoAcLearnItem.addCode()  }
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
        onCancelClicked: {    isShowhint=false  }
        onSureClicked:
        {
            isShowhint=false
           pLuxDomoAcLearnItem.deleteCode(pressCode,pressIndex)
        }
    }

    LuxDomoHintIrLearn
    {
        width: parent.width/2
        height: parent.height*2/5
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        enabled: isShowIrHint
        visible: isShowIrHint
        onClicked1:
        {
            pLuxDomoAcLearnItem.cancelLearn()
            isShowIrHint=false
        }
    }
}
