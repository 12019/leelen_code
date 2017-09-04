import  QtQuick 1.0
import   LuxDomoRoomControl  1.0
import   "../content"
Rectangle
{
    id:roomControl
    objectName: "roomControl"
    width: 1024
    height: 600
    color: "#00000000"

    property    string   tempValue: ""
    property    string   airValue: ""
    property    string   humidnessValue: ""
    property    bool      isShowTitle: true
    property    int         listW: 100
    property    string    pressIndex: "1"
    function    clearModel() {   model.clear()   }
    function    addModel(listData){   model.append({ "listData":listData })   }

    LuxDomoRoomControlItem
    {
        id:pLuxDomoRoomControl
        objectName: "pLuxDomoRoomControl"
    }

    Image
    {
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent
        source: "../room_control/room_background.png"
    }

    Rectangle
    {
        height: parent.height/8
        width: parent.width
        color: "#00000000"
        visible: isShowTitle
        Image
        {
            anchors.fill: parent
            source: "../room_control/top_background.png"
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
                    id:backImage
                    width:parent.width/6
                    height: parent.height*3/5
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width/8
                    x:0
                    source: "../room_control/back_off.png"
                }
                LuxDomoLabel
                {
                    width: parent.width/3
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: backImage.right
                    textName: "返回"
                    textSize:20
                    color:"#00000000"
                }
            }
            Item
            {
                width: parent.width*7/15
                height: parent.height
                LuxDomoLabel
                {
                    width: parent.width*3/7
                    height: parent.height
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width*3/7
                    textName: "家居控制"
                    textSize: 25
                }
            }
            Item
            {
                width: parent.width/3
                height: parent.height
                Row
                {
                    anchors.fill: parent
                    Item
                    {
                        width: parent.width/3
                        height: parent.height
                        Row
                        {
                            anchors.fill: parent
                            Item
                            {
                                width: 2
                                height: parent.height
                                Image
                                {
                                    width: parent.width
                                    height: parent.height*2/3
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    source: "../room_control/dividing_line.png"
                                }
                            }
                            Item
                            {
                                width: parent.width/3
                                height: parent.height
                                Image
                                {
                                    width: parent.width*4/5
                                    height: parent.height*2/3
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    source: "../room_control/temp.png"
                                }
                            }
                            Item
                            {
                                width: parent.width*2/3-2
                                height: parent.height
                                LuxDomoLabel
                                {
                                    anchors.fill: parent
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    textName: tempValue
                                    textColor: "white"
                                    textSize: 16
                                }
                            }
                        }
                    }
                    Item
                    {
                        width: parent.width/3
                        height: parent.height
                        Row
                        {
                            anchors.fill: parent
                            Item
                            {
                                width: 2
                                height: parent.height
                                Image
                                {
                                    width: parent.width
                                    height: parent.height*2/3
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    source: "../room_control/dividing_line.png"
                                }
                            }
                            Item
                            {
                                width: parent.width/3
                                height: parent.height
                                Image
                                {
                                    width: parent.width*4/5
                                    height: parent.height*2/3
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    source: "../room_control/humidness.png"
                                }
                            }
                            Item
                            {
                                width: parent.width*2/3-2
                                height: parent.height
                                LuxDomoLabel
                                {
                                    anchors.fill: parent
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    textName: humidnessValue
                                    textColor: "white"
                                    textSize: 16
                                }
                            }
                        }
                    }
                    Item
                    {
                        width: parent.width/3
                        height: parent.height
                        Row
                        {
                            anchors.fill: parent
                            Item
                            {
                                width: 2
                                height: parent.height
                                Image
                                {
                                    width: parent.width
                                    height: parent.height*2/3
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    source: "../room_control/dividing_line.png"
                                }
                            }
                            Item
                            {
                                width: parent.width/3
                                height: parent.height
                                Image
                                {
                                    width: parent.width*4/5
                                    height: parent.height*2/3
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    source: "../room_control/air.png"
                                }
                            }
                            Item
                            {
                                width: parent.width*2/3-4
                                height: parent.height
                                LuxDomoLabel
                                {
                                    anchors.fill: parent
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    textName: airValue
                                    textColor: "white"
                                    textSize: 16
                                }
                            }
                            Item
                            {
                                width: 2
                                height: parent.height
                                Image
                                {
                                    width: parent.width
                                    height: parent.height*2/3
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    source: "../room_control/dividing_line.png"
                                }
                            }
                        }
                    }
                }
            }
        }
    }

   ListModel
   {
       id: model
       ListElement  {  listData:"NULL"  }
   }

   Component
    {
        id:delegate
        Item
        {
            id:dataItem
            width: list.cellWidth
            height: list.cellHeight
            Rectangle
            {
                width: 10
                height: 10
                radius: 10
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                color:pressIndex==listData?"white":"#af000000"
            }
        }
   }

   GridView
   {
       id: list
       width: listW
       height: 30
       cellHeight: 30
       cellWidth: 15
       anchors.horizontalCenter: parent.horizontalCenter
       anchors.bottom: parent.bottom
       clip: true
       flow: GridView.LeftToRight
       model: model
       delegate:delegate
   }
}

