import  QtQuick 1.0
import   "../content"
Item
{
    width: 300
    height: 200

    signal  clicked1

    Image
    {
        anchors.fill: parent
        source: "../home_manage/irLearn_hint.png"
    }

    Column
    {
        anchors.fill: parent
        Item
        {
            width: parent.width
            height: parent.height/3
            Rectangle
            {
               width: parent.width*9/10
               height: 1
               anchors.bottom: parent.bottom
               anchors.horizontalCenter: parent.horizontalCenter
               color: "#bfbfbf"
            }
            LuxDomoLabel
            {
                anchors.fill: parent
                textName: "提示"
                textSize: 25
            }
        }
        Item
        {
            width: parent.width
            height: parent.height/3
            LuxDomoLabel
            {
                anchors.fill: parent
                textName: "用户使用遥控器按下相应按钮学习"
                textSize: 20
                textColor:"#676767"
            }
        }
        Item
        {
            width: parent.width
            height: parent.height/3
            LuxDomoButton
            {
                width: parent.width/3
                height: parent.height*2/3
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                name:"取消学习"
                onClicked: {   clicked1()   }
            }
         }
    }
}
