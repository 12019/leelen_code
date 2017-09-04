import QtQuick 1.0
Item
{
    id:hint
    width: 300
    height: 150

    signal  sureClicked
    signal  cancelClicked
    property string   textStr: "你确定要删除吗？"
    property string   imageStr: "../home_manage/delete_hint.png"

    MouseArea{ anchors.fill: parent }

    Image
    {
        anchors.fill: parent
        source: imageStr
    }

    Column
    {
        anchors.fill: parent
        Item
        {
           width: parent.width
           height: parent.height/2
           LuxDomoLabel
           {
               anchors.fill: parent
               textName: textStr
               textSize:22
           }
        }
        Item
        {
            width: parent.width
            height: parent.height/2
            Row
            {
                anchors.fill: parent
                Item
                {
                    width: parent.width/2
                    height: parent.height
                    LuxDomoButton
                    {
                        width: parent.width/2
                        height: parent.height/2
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        name:"取消"
                        fontsize: 20
                        onClicked: hint.cancelClicked()
                    }
                }
                Item
                {
                    width: parent.width/2
                    height: parent.height
                    LuxDomoButton
                    {
                        width: parent.width/2
                        height: parent.height/2
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        name:"确定"
                        fontsize: 20
                        onClicked: hint.sureClicked()
                    }
                }
            }
        }
    }
}

