import   QtQuick 1.0
import   LuxDomoPullDownList 1.0
Rectangle
{
    id:optionList
    objectName:"optionList"
    width: 450
    height: 480
    color:"#efeff4"

    property    int    pressIndex: -1
    property    string   dataType: ""
    property    string   listTitle: ""

    function    clearModel()      {    model.clear()   }
    function    addModel(addData){    model.append({ "listData":addData })  }


    LuxDomoPullDownListItem
    {
           id:pLuxDomoPullDownListItem
           objectName: "pLuxDomoPullDownListItem"
    }

   MouseArea {   anchors.fill: parent  }

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
                   listData:"NULL"
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
                LuxDomoLabel
                {
                    anchors.fill: parent
                    textName: listData
                }
               MouseArea
               {
                   anchors.fill: parent
                   onClicked:
                   {
                       pressIndex=index
                       pLuxDomoPullDownListItem.setDeviceData(listData)
                   }
               }
        }
   }

   Rectangle
   {
       width: optionList.width*4/5
       height:optionList.height*6/7
       anchors.top: title.bottom
       anchors.horizontalCenter: optionList.horizontalCenter
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
