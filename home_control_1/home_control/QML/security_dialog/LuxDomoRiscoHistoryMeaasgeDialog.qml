import  QtQuick 1.0
import  "../content"
import  LuxDomoRiscoHistoryMeaasge  1.0
Rectangle
{
    id:message
    objectName: "message"
    width: 896
    height: 465
    color:"#efeff4"

    property   string    condition: "allMessage"

    MouseArea{   anchors.fill: parent  }

    LuxDomoRiscoHistoryMeaasgeItem
    {
         id:pLuxDomoRiscoHistoryMeaasgeItem
         objectName: "pLuxDomoRiscoHistoryMeaasgeItem"
    }

    Component.onCompleted: {  pLuxDomoRiscoHistoryMeaasgeItem.init()  }

    Row
    {
        width: parent.width
        height: parent.height/6
        anchors.bottom: parent.bottom
        Rectangle
        {
            width: parent.width/3
            height: parent.height
            color: condition=="allMessage"?"white":"#cccccc"
            Rectangle
            {
                height: parent.height
                width: 2
                anchors.right: parent.right
                color: "white"
            }
            LuxDomoLabel
            {
                anchors.fill: parent
                textName: " 全部事件"
                textSize: 25
                textColor: condition=="allMessage"?"#24c3be":"#151412"
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    condition="allMessage"
                    pLuxDomoRiscoHistoryMeaasgeItem.showEvent("allEvent")
                }
            }
        }
        Rectangle
        {
            width: parent.width/3
            height: parent.height
            color: condition=="warnMessage"?"white":"#cccccc"
            Rectangle
            {
                height: parent.height
                width: 2
                anchors.right: parent.right
                color: "white"
            }
            Row
            {
                anchors.fill: parent
                Item
                {
                      width: parent.width/2
                      height: parent.height
                      Image
                      {
                          width: parent.height*2/5
                          height: parent.height*2/5
                          anchors.verticalCenter: parent.verticalCenter
                          anchors.right: parent.right
                          source: condition=="warnMessage"?"../security/risco_warn_down.png":"../security/risco_warn_up.png"
                      }
                }
                Item
                {
                    width: parent.width/2
                    height: parent.height
                    LuxDomoLabel1
                    {
                        anchors.fill: parent
                        textName:"故障"
                        textSize: 25
                        margin: 10
                        textColor: condition=="warnMessage"?"#24c3be":"#151412"
                    }
                }
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    condition="warnMessage"
                    pLuxDomoRiscoHistoryMeaasgeItem.showEvent("troubleEvent")
                }
            }
        }
        Rectangle
        {
            width: parent.width/3
            height: parent.height
            color: condition=="stoppageMessage"?"white":"#cccccc"
            Row
            {
                anchors.fill: parent
                Item
                {
                      width: parent.width/2
                      height: parent.height
                      Image
                      {
                          width: parent.height*2/5
                          height: parent.height*2/5
                          anchors.verticalCenter: parent.verticalCenter
                          anchors.right: parent.right
                          source: condition=="stoppageMessage"?"../security/risco_stoppage_down.png":"../security/risco_stoppage_up.png"
                      }
                }
                Item
                {
                    width: parent.width/2
                    height: parent.height
                    LuxDomoLabel1
                    {
                        anchors.fill: parent
                        textName:"报警"
                        textSize: 25
                        textColor: condition=="stoppageMessage"?"#24c3be":"#151412"
                        margin: 10
                    }
                }
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    condition="stoppageMessage"
                    pLuxDomoRiscoHistoryMeaasgeItem.showEvent("alarmEvent")
                }
            }
        }
    }

    function    clearModel()            {   model.clear()   }
    function    addModel(addData){  model.append({ "listData":addData }) }

    ListModel
    {
        id: model
        ListElement
        {
            listData:""
        }
    }

    Component
     {
         id:delegate
         Rectangle
         {
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
             LuxDomoLabel1
             {
                 anchors.fill: parent
                 margin: parent.width/20
                 textName:listData
                 textSize: 20
             }
         }
     }

    ListView
    {
        id: list
        width: parent.width
        height: parent.height*5/6
        anchors.top: parent.top
        clip: true
        model: model
        delegate:delegate
     }
}
