import  QtQuick 1.0
import  LuxDomoStyleList  1.0
Rectangle
{
    id:styleList
    objectName:"styleList"
    width: 450
    height: 480
    color:"#efeff4"

    property    int    pressIndex: -1
    property    string   listTitle: ""

    function    clearModel() {   model.clear()   }
    function    addModel(listImage,listData){   model.append({ "listImage":listImage,"listData":listData })   }

    MouseArea {   anchors.fill: parent    }

    LuxDomoStyleListItem
    {
        id:pLuxDomoStyleListItem
        objectName: "pLuxDomoStyleListItem"
    }

    Item
    {
        id:title
        width: parent.width
        height: parent.height/7
        LuxDomoLabel
        {
            width: parent.width/2
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            textName: listTitle
            textSize: 20
        }
    }

    ListModel
    {
        id: model
        ListElement
        {
            listImage:""
            listData:""
        }
    }

    Component
     {
         id:delegate
         Rectangle
         {
             id:dataItem
             width: list.width
             height: list.height/6
             color: pressIndex==index?"#24c3be":"white"
             Rectangle
             {
                width: parent.width*7/8
                height: 1
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                color: "#bfbfbf"
             }
             Row
             {
                 anchors.fill: parent
                 Item
                 {
                     width: parent.width/6
                     height: parent.height
                 }
                 Item
                 {
                      width: parent.width/3
                      height: parent.height
                      Image
                      {
                         height: parent.height*3/5
                         width: parent.height*3/5
                         anchors.verticalCenter: parent.verticalCenter
                         anchors.horizontalCenter: parent.horizontalCenter
                         source: listImage
                      }
                 }
                 Item
                 {
                     width: parent.width/3
                     height: parent.height
                     LuxDomoLabel
                     {
                         anchors.fill: parent
                         textName:listData
                     }
                     MouseArea
                     {
                         anchors.fill: parent
                         onClicked:
                         {
                             pressIndex=index
                             pLuxDomoStyleListItem.setDeviceData(listData)
                         }
                     }
                 }
             }
        }
   }

    Rectangle
    {
        width: parent.width*4/5
        height:parent.height*6/7
        anchors.top: title.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        radius: 20
        ListView
        {
            id: list
            anchors.fill: parent
            clip: true
            model: model
            delegate:delegate
        }
    }
}

