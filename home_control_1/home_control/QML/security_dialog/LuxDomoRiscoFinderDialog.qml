import   QtQuick 1.0
import   "../content"
import  LuxDomoRiscoFinder  1.0
Rectangle
{
    id:finder
    objectName: "finder"
    width: 896
    height: 465
    color:"#efeff4"

    function    clearModel()    {  model.clear()   }
    function    addModel(partitionNumber,defenceAreaNumber,defenceAreaBypass) {
                                        model.append({ "partitionNumber":partitionNumber,
                                                                     "defenceAreaNumber":defenceAreaNumber,
                                                                     "defenceAreaBypass":defenceAreaBypass }) }
    function     changModel(index,propertyName,propertyValue){  model.setProperty( index,propertyName,propertyValue) }

    property   int         partition: -1
    property   int         defenceArea: -1
    property   string    bypassValue: ""
    property   int         pressIndex: -1
    property   bool      showBypassDialog: false
    property   string    bypassText: ""

    Component.onCompleted: {  pLuxDomoRiscoFinderItem.init()  }

    MouseArea{  anchors.fill: parent   }

    LuxDomoRiscoFinderItem
    {
        id:pLuxDomoRiscoFinderItem
        objectName: "pLuxDomoRiscoFinderItem"
    }

    ListModel
    {
        id: model
        ListElement
        {
            partitionNumber:""
            defenceAreaNumber:""
            defenceAreaBypass:""
        }
    }

    Component
     {
         id:delegate
         Rectangle
         {
             width:list.width
             height: list.height/6
             Rectangle
             {
                 width: parent.width*7/8
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
                     height: parent.height
                     width: parent.width/8
                     Image
                     {
                         width: parent.height*3/5
                         height: parent.height*3/5
                         anchors.verticalCenter: parent.verticalCenter
                         anchors.horizontalCenter: parent.horizontalCenter
                         source: "../security/security_finder1.png"
                     }
                 }
                 Item
                 {
                     width: parent.width/8
                     height: parent.height
                     LuxDomoLabel
                     {
                         anchors.fill: parent
                         textName: partitionNumber+"分区:"
                         textSize: 23
                     }
                 }
                Item
                {
                    width: parent.width/8
                    height: parent.height
                    LuxDomoLabel1
                    {
                        anchors.fill: parent
                        textName: "防区"+defenceAreaNumber
                        textSize: 20
                    }
                }
             }
             MouseArea
             {
                 anchors.fill: parent
                 onClicked:
                 {
                     showBypassDialog=true
                     partition=partitionNumber
                     defenceArea=defenceAreaNumber
                     pressIndex=index
                     if(defenceAreaBypass=="on")
                     {
                         bypassValue="off"
                         bypassText="确定解除分区 ' "+partitionNumber+" ' 防区 ' "+defenceAreaNumber+" ' 旁路吗?"
                     }
                     else
                     {
                         bypassValue="on"
                         bypassText="确定旁路分区 ' "+partitionNumber+" ' 防区 ' "+defenceAreaNumber+" ' 吗?"
                     }
                 }
             }
         }
     }

    ListView
    {
        id: list
        anchors.fill: parent
        clip: true
        model: model
        delegate:delegate
     }

    Item
    {
        width: parent.width
        height: parent.height
        visible: showBypassDialog
        enabled: showBypassDialog
        MouseArea
        {
            anchors.fill: parent
            onClicked: {   showBypassDialog=false  }
        }
        LuxDomoRiscoBypassHint
        {
            width: parent.width/2
            height: parent.height*2/3
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            message: bypassText
            onCancelClicked: {   showBypassDialog=false  }
            onSureClicked:
            {
                 showBypassDialog=false
                pLuxDomoRiscoFinderItem.bypassDefenceArea(pressIndex,partition,defenceArea,bypassValue)
            }
        }
    }
}
