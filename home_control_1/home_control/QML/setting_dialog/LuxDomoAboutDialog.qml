import  QtQuick 1.0
import  "../content"
import   LuxDomoAboutSetting 1.0
Rectangle
{
    id:aboutSetting
    objectName: "aboutSetting"
    width: 450
    height: 600
    color:"#efeff4"

    function    clearModel()      {  model.clear()  }
    function    addModel(listData,listImage1,listImage2){ model.append({ "listData":listData,"listImage1":listImage1,"listImage2":listImage2 })}

     Component.onCompleted: { pLuxDomoAboutSettingItem.init()  }

    LuxDomoAboutSettingItem
    {
        id:pLuxDomoAboutSettingItem
        objectName: "pLuxDomoAboutSettingItem"
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
            textName: "场景设置"
            textSize: 25
        }
    }

    Column
    {
        width: parent.width
        height: parent.height/4
        y:parent.height/8
        Item
        {
            width: parent.width
            height: parent.height/2
            LuxDomoLabel3
            {
                width: parent.width*9/10
                height: parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                textName: "LuxDomo智能家居功能介绍"
                textSize: 30
            }
        }
        Item
        {
            width: parent.width
            height: parent.height/2
            LuxDomoLabel2
            {
                width: parent.width*9/10
                height: parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                textName: "网址：http://www.leelen.com"
                textSize: 20
                textColor: "#24c3be"
                margin: 0
            }
            Image
            {
                width: parent.width*9/10
                height: 2
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                source: "../line.png"
            }
        }
    }

    ListModel
    {
        id: model
        ListElement
        {
            listData:"NULL"
            listImage1:""
            listImage2:""
        }
    }

    Component
     {
         id:delegate
         Item
         {
             width: list.width
             height: list.height
             Column
             {
                 anchors.fill: parent
                 Item
                 {
                     width: parent.width
                     height: parent.height/4
                     LuxDomoLabel2
                     {
                         anchors.fill: parent
                         textName: listData
                         textSize: 23
                     }
                 }
                 Item
                 {
                     width: parent.width
                     height: parent.height*3/4
                     Image
                     {
                         width: parent.width*3/7
                         height: parent.height
                         anchors.left: parent.left
                         source: listImage1
                     }
                     Image
                     {
                         width: parent.width*3/7
                         height: parent.height
                         anchors.right: parent.right
                         source: listImage2
                     }
                 }
             }
         }
     }

    ListView
    {
        id: list
        width: parent.width*9/10
        height: parent.height/2
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/16
        anchors.horizontalCenter: parent.horizontalCenter
        clip: true
        model: model
        delegate: delegate
    }
}

