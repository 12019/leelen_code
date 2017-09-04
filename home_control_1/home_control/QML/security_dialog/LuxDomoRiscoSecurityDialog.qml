import  QtQuick 1.0
import   "../content"
import   LuxDomoRiscoSecurity  1.0
Rectangle
{
    id:security
    objectName: "security"
    width: 896
    height: 465
    color:"#efeff4"

    Component.onCompleted: {   pLuxDomoRiscoSecurityItem.init()  }

    MouseArea{  anchors.fill: parent }

    LuxDomoRiscoSecurityItem
    {
        id:pLuxDomoRiscoSecurityItem
        objectName: "pLuxDomoRiscoSecurityItem"
    }

    Row
    {
        width: parent.width
        height: parent.height/3
        Item
        {
            width: parent.width/3
            height: parent.height
            Image
            {
                width: parent.height*3/5
                height: parent.height*3/5
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                source: "../security/risco_defence.png"
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: { pLuxDomoRiscoSecurityItem.controlAllDefence("on") }
                }
            }
            LuxDomoLabel
            {
                width: parent.width
                height: parent.height/5
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                textName: "布防"
            }
        }
        Item
        {
            width: parent.width/3
            height: parent.height
            Image
            {
                width: parent.height*3/5
                height: parent.height*3/5
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                source:"../security/risco_wait_defence.png"
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: { pLuxDomoRiscoSecurityItem.controlAllDefence("wait") }
                }
            }
            LuxDomoLabel
            {
                width: parent.width
                height: parent.height/5
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                textName:"留守布防"
            }
        }
        Item
        {
            width: parent.width/3
            height: parent.height
            Image
            {
                width: parent.height*3/5
                height: parent.height*3/5
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                source: "../security/risco_not_defence.png"
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: { pLuxDomoRiscoSecurityItem.controlAllDefence("off") }
                }
            }
            LuxDomoLabel
            {
                width: parent.width
                height: parent.height/5
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                textName:"撤防"
            }
        }
    }

    function    clearModel()    {  model.clear()   }
    function    addModel(partitionNumber,defence){  model.append({ "partitionNumber":partitionNumber,"defence":defence }) }
    function    changModel(index,propertyName,propertyValue){  model.setProperty( index,propertyName,propertyValue) }

    ListModel
    {
        id: model
        ListElement
        {
            partitionNumber:""
            defence:""
        }
    }

    Component
     {
         id:delegate
         Item
         {
             width: list.width
             height: list.height/3
             LuxDomoLabel
             {
                 width: parent.width
                 height: parent.height*2/3
                 anchors.top: parent.top
                 textName:"分区"+ partitionNumber
                 textSize: 23
             }
             Image
             {
                 width: parent.width*2/3
                 height: 6
                 anchors.horizontalCenter: parent.horizontalCenter
                 y:parent.height*3/4-3
                 source: "../security/risco_line.png"
             }
             Row
             {
                 width: parent.width
                 height: parent.height/2
                 anchors.bottom: parent.bottom
                 Item
                 {
                     width: parent.width/3
                     height: parent.height
                     Image
                     {
                         width: parent.height
                         height: parent.height
                         anchors.horizontalCenter: parent.horizontalCenter
                         source:defence=="on"?"../security/risco_yellow.png":"../security/risco_white.png"
                         MouseArea
                         {
                             anchors.fill: parent
                             onClicked: {  pLuxDomoRiscoSecurityItem.changDefence(index,partitionNumber,"on") }
                         }
                     }
                 }
                 Item
                 {
                     width: parent.width/3
                     height: parent.height
                     Image
                     {
                         width: parent.height
                         height: parent.height
                         anchors.horizontalCenter: parent.horizontalCenter
                         source: defence=="wait"?"../security/risco_blue.png":"../security/risco_white.png"
                         MouseArea
                         {
                             anchors.fill: parent
                             onClicked: {  pLuxDomoRiscoSecurityItem.changDefence(index,partitionNumber,"wait") }
                         }
                     }
                 }
                 Item
                 {
                     width: parent.width/3
                     height: parent.height
                     Image
                     {
                         width: parent.height
                         height: parent.height
                         anchors.horizontalCenter: parent.horizontalCenter
                         source:defence=="off"?"../security/risco_cyan.png":"../security/risco_white.png"
                         MouseArea
                         {
                             anchors.fill: parent
                             onClicked: {  pLuxDomoRiscoSecurityItem.changDefence(index,partitionNumber,"off") }
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
        height: parent.height*2/3
        anchors.bottom: parent.bottom
        clip: true
        model: model
        delegate: delegate
    }
}

