import     QtQuick 1.0
import     LuxDomoDeviceOption 1.0
import     "../content"
Rectangle
{
    id:deviceTypeOption
    objectName: "deviceOption"
    width: 700
    height: 480
    color:"#efeff4"

    property  string  optionType: ""
    property  string  optionAddress: ""
    property  string  optionAttribute: ""
    property  string  optionName: ""

    function  setTypeData(data){   pLuxDomoDeviceOptionItem.setTypeData(data)  }

    LuxDomoDeviceOptionItem
    {
        id:pLuxDomoDeviceOptionItem
        objectName: "LuxDomoDeviceOptionItem"
    }

   MouseArea{   anchors.fill: parent   }

    Rectangle
    {
        id:manageTitle
        width: parent.width
        height:parent.height/8
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
                      source: "../home_manage/back_on.png"
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
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: {   pLuxDomoDeviceOptionItem.close()  }
                }
            }
            Item
            {
                width: parent.width*4/5
                height: parent.height
                LuxDomoLabel
                {
                    width: parent.width/4
                    height: parent.height
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width/4
                    textName:"添加设备"
                    textSize: 25
                }
            }
        }
     }

    Rectangle
    {
        id:manageItem
        width: parent.width*5/14
        height: parent.height*3/8
        anchors.top: manageTitle.bottom
        anchors.topMargin: parent.height/24
        Rectangle
        {
           width: parent.width
           height: 1
           anchors.bottom: parent.bottom
           color: "#bfbfbf"
        }
        Rectangle
        {
           width: parent.width
           height: 1
           anchors.top: parent.top
           color: "#bfbfbf"
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
                    width: parent.width
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
                        width: parent.width/4
                        height: parent.height
                        LuxDomoLabel
                        {
                            anchors.fill: parent
                            textName: "类型"
                            textColor: "#676767"
                            textSize: 20
                        }
                    }
                    Item
                    {
                        width: parent.width/2
                        height: parent.height
                        LuxDomoLabel
                        {
                            anchors.fill: parent
                             textName: deviceTypeOption.optionType
                            textSize: 20
                        }
                        MouseArea
                        {
                            anchors.fill: parent
                            onClicked: {  pLuxDomoDeviceOptionItem.showPullDownList("typeButton")  }
                        }
                    }
                    Item
                    {
                        width: parent.width/4
                        height: parent.height
                        Image
                        {
                            width: parent.height/3
                            height:  parent.height/3
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            source: "../home_manage/goto.png"
                        }
                    }
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/3
                Rectangle
                {
                    width: parent.width
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
                        width: parent.width/4
                        height: parent.height
                        LuxDomoLabel
                        {
                            anchors.fill: parent
                            textName: "属性"
                            textColor: "#676767"
                            textSize: 20
                        }
                    }
                    Item
                    {
                        width: parent.width/2
                        height: parent.height
                        LuxDomoLabel
                        {
                            anchors.fill: parent
                            textName: deviceTypeOption.optionAttribute
                            textSize: 20
                        }
                        MouseArea
                        {
                            anchors.fill: parent
                            onClicked: {   pLuxDomoDeviceOptionItem.showPullDownList("attributeButton") }
                        }
                    }
                    Item
                    {
                        width: parent.width/4
                        height: parent.height
                        Image
                        {
                            width: parent.height/3
                            height:  parent.height/3
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            source: "../home_manage/goto.png"
                        }
                    }
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/3
                Rectangle
                {
                    width: parent.width
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
                        width: parent.width/4
                        height: parent.height
                        LuxDomoLabel
                        {
                            anchors.fill: parent
                            textName: "地址"
                            textColor: "#676767"
                            textSize: 20
                        }
                    }
                    Item
                    {
                        width: parent.width/2
                        height: parent.height
                        LuxDomoLabel
                        {
                            anchors.fill: parent
                            textName: deviceTypeOption.optionAddress
                            textSize: 20
                        }
                        MouseArea
                        {
                            anchors.fill: parent
                            onClicked: {  pLuxDomoDeviceOptionItem.showPullDownList("addressButton")  }
                        }
                    }
                    Item
                    {
                        width: parent.width/4
                        height: parent.height
                        Image
                        {
                            width: parent.height/3
                            height:  parent.height/3
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            source: "../home_manage/goto.png"
                        }
                    }
                }
            }
        }
    }

    Rectangle
    {
        width: parent.width*5/14
        height: parent.height/8
        anchors.top: manageItem.bottom
        anchors.topMargin: parent.height/24
        Rectangle
        {
           width: parent.width
           height: 1
           anchors.bottom: parent.bottom
           color: "#bfbfbf"
        }
        Rectangle
        {
           width: parent.width
           height: 1
           anchors.top: parent.top
           color: "#bfbfbf"
        }

        Row
        {
            anchors.fill: parent
            Item
            {
                width: parent.width/4
                height: parent.height
                LuxDomoLabel
                {
                    anchors.fill: parent
                    textName: "名称"
                    textColor: "#676767"
                    textSize: 20
                }
            }
            Item
            {
                width: parent.width*3/4
                height: parent.height
                LuxDomoLabel
                {
                    anchors.fill: parent
                    textName: deviceTypeOption.optionName
                    textSize: 20
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: {  pLuxDomoDeviceOptionItem.inputData("DeviceName",deviceTypeOption.optionName) }
                }
            }
        }
    }

    Item
    {
        width: parent.width*5/14
        height: parent.height*5/16
        anchors.bottom: parent.bottom
        LuxDomoButton
        {
            width: parent.width*2/3
            height: parent.height/3
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            name:"确定"
            fontsize: 22
            onClicked: {   pLuxDomoDeviceOptionItem.ensure()   }
        }
    }

    Rectangle
    {
        width: 2
        height: parent.height*7/8
        x:parent.width*5/14-2
        y:parent.height/8
        color: "#9fa0a4"
    }
}
