import   QtQuick 1.0
import    LuxDomoVersionsSettting  1.0
import    "../content"
Rectangle
{
    id:versionsSetting
    objectName: "versionsSetting"
    width: 450
    height: 480
    color:"#efeff4"

    property string   newVersion: "LuxDomo_1.0"
    property string   oldVersion: "LuxDomo_1.0"

    LuxDomoVersionsSetttingItem
    {
        id:pLuxDomoVersionsSetttingItem
        objectName: "pLuxDomoVersionsSetttingItem"
    }

    MouseArea{    anchors.fill: parent    }

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
            textName: "版本更新"
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
            anchors.fill: parent
            Item
            {
                width: parent.width
                height: parent.height*4/7
                LuxDomoButton
                {
                     height: parent.height*3/5
                     width: parent.height*3/5
                     anchors.verticalCenter: parent.verticalCenter
                     anchors.horizontalCenter: parent.horizontalCenter
                     image:"../setting/version.png"
                     imagepressed:"../setting/version.png"
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/6
                Row
                {
                    width: parent.width*4/5
                    height: parent.height/2
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    Item
                    {
                         width: parent.width*2/5
                         height: parent.height
                         LuxDomoLabel
                         {
                             anchors.fill: parent
                             textName: "当前版本"
                             textSize: 22
                         }
                    }
                    Item
                    {
                        width: parent.width*3/5
                        height: parent.height
                        Image
                        {
                            anchors.fill: parent
                            source: "../setting/version_number.png"
                        }
                        LuxDomoLabel
                        {
                            anchors.fill: parent
                            textName: oldVersion
                            textSize: 22
                        }
                    }
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/6
                Row
                {
                    width: parent.width*4/5
                    height: parent.height/2
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    Item
                    {
                         width: parent.width*2/5
                         height: parent.height
                         LuxDomoLabel
                         {
                             anchors.fill: parent
                             textName: "最新版本"
                             textSize: 22
                         }
                    }
                    Item
                    {
                        width: parent.width*3/5
                        height: parent.height
                        Image
                        {
                            anchors.fill: parent
                            source: "../setting/version_number.png"
                        }
                        LuxDomoLabel
                        {
                            anchors.fill: parent
                            textName: newVersion
                            textSize: 22
                        }
                    }
                }
            }
        }
    }
}

