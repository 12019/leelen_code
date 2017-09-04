import  QtQuick 1.0
import  "../content"
import  LuxDomoSynchronization  1.0
Rectangle
{
    id:synchronization
    objectName: "synchronization"
    width: 450
    height: 480
    color:"#efeff4"

    property string   result: ""
    property string   describe: ""

    MouseArea{  anchors.fill: parent  }

    LuxDomoSynchronizationItem
    {
        id:pLuxDomoSynchronizationItem
        objectName: "pLuxDomoSynchronizationItem"
    }

    Rectangle
    {
        width: parent.width
        height: parent.height/8
        color:"#f6f6f7"
        Rectangle
        {
           width: parent.width
           height: 1
           anchors.bottom: parent.bottom
           color: "#bfbfbf"
        }
        LuxDomoLabel
        {
            anchors.fill: parent
            textName: "文件同步"
            textSize: 25
        }
    }

    Item
    {
        width: parent.width
        height: parent.height*7/8
        anchors.bottom: parent.bottom
        Column
        {
            width: parent.width*7/8
            height: parent.height*19/20
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            Item
            {
                width: parent.width
                height: parent.height/8
                LuxDomoLabel1
                {
                    anchors.fill: parent
                    textName: "何为文件同步:"
                    textSize: 22
                    textColor: "#676767"
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/2
                LuxDomoLabel5
                {
                    anchors.fill: parent
                    textName: describe
                    textSize: 20
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/10
                LuxDomoLabel1
                {
                    anchors.fill: parent
                    textName: "阅读上述,若继续点击同步:"
                    textSize: 22
                    textColor: "#676767"
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/4
                LuxDomoButton
                {
                    width: parent.width/2
                    height: parent.height*2/5
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    name:"同步"
                    fontsize: 24
                    onClicked: {  pLuxDomoSynchronizationItem.fileSynchronization() }
                }
                LuxDomoLabel
                {
                    width: parent.width/4
                    height: parent.height*2/5
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    textName: result
                    textSize: 20
                    textColor: "red"
                }
            }
        }
    }
}
