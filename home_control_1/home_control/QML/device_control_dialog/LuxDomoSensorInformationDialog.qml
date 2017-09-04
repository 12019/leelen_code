import QtQuick 1.0
import LuxDomoSensorInformation 1.0
import "../content"
Rectangle
{
    id:sensorInformation
    objectName: "sensorInformation"
    width: 800
    height: 480
    color: "#00000000"

    property    string     tempValue: "0"
    property    string     airValue: "0"
    property    double   airValue1: 0
    property    string     humidnessValue: "0"
    property    string     deviceUid: "0"
    function    init(roomUid,deviceUid) { pLuxDomoSensorInformationItem.init(roomUid,deviceUid)  }

    LuxDomoSensorInformationItem
    {
        id:pLuxDomoSensorInformationItem
        objectName: "pLuxDomoSensorInformationItem"
    }

    MouseArea
    {
        anchors.fill: parent
        onClicked: {  pLuxDomoSensorInformationItem.close()  }
    }

    Item
    {
        width: parent.width*3/5
        height: parent.height*13/16
        anchors.horizontalCenter: parent.horizontalCenter
        y:parent.height*5/32
        Image
        {
            anchors.fill: parent
            source: "../device_control/sensorBackground.png"
        }
        MouseArea{  anchors.fill: parent  }
        Column
        {
            anchors.fill: parent
            Item
            {
                width: parent.width
                height: parent.height*2/10
                LuxDomoLabel
                {
                    anchors.fill: parent
                    textColor: "#000000"
                    textSize: 25
                    textName: "温湿度及PM2.5详情"
                }
                Rectangle
                {
                    width: parent.width*9/10
                    height: 1
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    color: "#b2b2b2"
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/10
                Row
                {
                    anchors.fill: parent
                    Item
                    {
                        width: parent.width/8
                        height: parent.height
                    }
                    Item
                    {
                        width: parent.width/12
                        height: parent.height
                        Image
                        {
                            width: parent.width*3/7
                            height: parent.height*3/4
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            source: "../device_control/temp.png"
                        }
                    }
                    Item
                    {
                        width: parent.width/9
                        height: parent.height
                        LuxDomoLabel3
                        {
                            anchors.fill: parent
                            textName: "温度:"
                            textSize: 22
                        }
                    }
                    Item
                    {
                        width: parent.width/6
                        height: parent.height
                        LuxDomoLabel3
                        {
                            anchors.fill: parent
                            textSize: 22
                            textColor: "#14b3ad"
                            textName: tempValue
                        }
                    }
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/10
                Row
                {
                    anchors.fill: parent
                    Item
                    {
                        width: parent.width/8
                        height: parent.height
                    }
                    Item
                    {
                        width: parent.width/12
                        height: parent.height
                        Image
                        {
                            width: parent.width*3/7
                            height: parent.height*3/4
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            source: "../device_control/humidness.png"
                        }
                    }
                    Item
                    {
                        width: parent.width/9
                        height: parent.height
                        LuxDomoLabel1
                        {
                            anchors.fill: parent
                            textName: "湿度:"
                            textSize: 22
                        }
                    }
                    Item
                    {
                        width: parent.width/6
                        height: parent.height
                        LuxDomoLabel1
                        {
                            anchors.fill: parent
                            textSize: 22
                            textColor: "#14b3ad"
                            textName: humidnessValue
                        }
                    }
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/10
                Row
                {
                    anchors.fill: parent
                    Item
                    {
                        width: parent.width/8
                        height: parent.height
                    }
                    Item
                    {
                        width: parent.width/12
                        height: parent.height
                        Image
                        {
                            width: parent.width*3/7
                            height: parent.height*3/4
                            anchors.top:parent.top
                            anchors.horizontalCenter: parent.horizontalCenter
                            source: "../device_control/air.png"
                        }
                    }
                    Item
                    {
                        width: parent.width/4+20
                        height: parent.height
                        LuxDomoLabel4
                        {
                            anchors.fill: parent
                            textName: "空气质量指数:"
                            textSize: 22
                        }
                    }
                    Item
                    {
                        width: parent.width/6
                        height: parent.height
                        LuxDomoLabel4
                        {
                            anchors.fill: parent
                            textSize: 22
                            textColor: "#14b3ad"
                            textName: airValue1.toString()+"("+airValue+")"
                        }
                    }
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/10
                LuxDomoLabel
                {
                    width: parent.width/2
                    height: parent.height
                    anchors.left: parent.left
                    textColor: "#333333"
                    textName: "0~35 '优'"
                    textSize: 20
                }
                LuxDomoLabel
                {
                    width: parent.width/2
                    height: parent.height
                    anchors.right: parent.right
                    textColor: "#333333"
                    textName: "36~75 '良'"
                    textSize: 20
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/10
                LuxDomoLabel
                {
                    width: parent.width/2
                    height: parent.height
                    anchors.left: parent.left
                    textColor: "#333333"
                    textName: "76~115 '轻度污染'"
                    textSize: 20
                }
                LuxDomoLabel
                {
                    width: parent.width/2
                    height: parent.height
                    anchors.right: parent.right
                    textColor: "#333333"
                    textName: "116~150  '中度污染'"
                    textSize: 20
                }

            }
            Item
            {
                width: parent.width
                height: parent.height/10
                LuxDomoLabel
                {
                    width: parent.width/2
                    height: parent.height
                    anchors.left: parent.left
                    textColor: "#333333"
                    textName: "151~250  '重度污染'"
                    textSize: 20
                }
                LuxDomoLabel
                {
                    width: parent.width/2
                    height: parent.height
                    anchors.right: parent.right
                    textColor: "#333333"
                    textName: "251~500  '严重污染'"
                    textSize: 20
                }
            }
            Item
            {
                width: parent.width
                height: parent.height*2/10
                Row
                {
                    width: parent.width*9/10
                    height: parent.height/2
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    Item
                    {
                        width: parent.width*7/100
                        height:parent.height
                        Rectangle
                        {
                            width: parent.width
                            height: parent.height/2
                            color:"#0bce27"
                            radius: 8
                        }
                        Rectangle
                        {
                            width: parent.width/2
                            height: parent.height/2
                            anchors.right: parent.right
                            color:"#0bce27"
                        }
                        LuxDomoLabel
                        {
                            width: parent.width/3
                            height: parent.height/2
                            anchors.left:parent.left
                            anchors.bottom: parent.bottom
                            textName: "0"
                            textColor: "#333333"
                            textSize: 20
                        }
                    }
                    Item
                    {
                        width: parent.width*8/100
                        height:parent.height
                        Rectangle
                        {
                            width: parent.width
                            height: parent.height/2
                            anchors.right: parent.right
                            color:"#a2ce0b"
                        }
                        LuxDomoLabel
                        {
                            width: parent.width/3
                            height: parent.height/2
                            anchors.horizontalCenter: parent.left
                            anchors.bottom: parent.bottom
                            textName: "35"
                            textColor: "#333333"
                            textSize: 20
                        }
                    }
                    Item
                    {
                        width: parent.width*8/100
                        height:parent.height
                        Rectangle
                        {
                            width: parent.width
                            height: parent.height/2
                            anchors.right: parent.right
                            color:"#ffd400"
                        }
                        LuxDomoLabel
                        {
                            width: parent.width/3
                            height: parent.height/2
                            anchors.horizontalCenter: parent.left
                            anchors.bottom: parent.bottom
                            textName: "75"
                            textColor: "#333333"
                            textSize: 20
                        }
                    }
                    Item
                    {
                        width: parent.width*7/100
                        height:parent.height
                        Rectangle
                        {
                            width: parent.width
                            height: parent.height/2
                            anchors.right: parent.right
                            color:"#ff9500"
                        }
                        LuxDomoLabel
                        {
                            width: parent.width/3
                            height: parent.height/2
                            anchors.horizontalCenter: parent.left
                            anchors.bottom: parent.bottom
                            textName: "115"
                            textColor: "#333333"
                            textSize: 20
                        }
                    }
                    Item
                    {
                        width: parent.width*20/100
                        height:parent.height
                        Rectangle
                        {
                            width: parent.width
                            height: parent.height/2
                            anchors.right: parent.right
                            color:"#ff4040"
                        }
                        LuxDomoLabel
                        {
                            width: parent.width/3
                            height: parent.height/2
                            anchors.horizontalCenter: parent.left
                            anchors.bottom: parent.bottom
                            textName: "150"
                            textColor: "#333333"
                            textSize: 20
                        }
                    }
                    Item
                    {
                        width: parent.width*50/100
                        height:parent.height
                        Rectangle
                        {
                            width: parent.width
                            height: parent.height/2
                            anchors.right: parent.right
                            color:"#8c0023"
                            radius: 8
                        }
                        Rectangle
                        {
                            width: parent.width/2
                            height: parent.height/2
                            anchors.left: parent.left
                            color:"#8c0023"
                        }
                        LuxDomoLabel
                        {
                            width: parent.width/3
                            height: parent.height/2
                            anchors.horizontalCenter: parent.left
                            anchors.bottom: parent.bottom
                            textName: "250"
                            textColor: "#333333"
                            textSize: 20
                        }
                        LuxDomoLabel
                        {
                            width: parent.width/7
                            height: parent.height/2
                            anchors.right:parent.right
                            anchors.bottom: parent.bottom
                            textName: "500"
                            textColor: "#333333"
                            textSize: 20
                        }
                    }
                }
                Image
                {
                    width:15
                    height: parent.height*2/5
                    y:parent.height/8
                    x:(airValue1/500)*parent.width*9/10+parent.width/20-5
                    source: "../device_control/cursor.png"
                }
            }
        }
    }
}
