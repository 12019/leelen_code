import   QtQuick 1.0
import   LuxDomoList  1.0
import   "../content"
Rectangle
{
    id:optionList
    objectName:"optionList"
    width: 240
    height: 200
   color:"#b3b3b3"

    property    int        pressIndex: -1
    property    string   dataType: ""

    function    clearModel()      {    model.clear()   }
    function    addModel(addData){    model.append({ "listData":addData })  }

    LuxDomoListItem
    {
        id:pLuxDomoListItem
        objectName: "pLuxDomoListItem"
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
             Item
             {
                 id:dataItem
                 width: list.width
                 height: list.height/3
                 Rectangle
                 {
                     anchors.fill: parent
                     color: optionList.pressIndex==index?"#24c3be":"#b3b3b3"
                     LuxDomoLabel
                     {
                         anchors.fill: parent
                         textName: listData
                         textSize: 22
                         textColor:  "#151412"
                     }
                     MouseArea
                     {
                         anchors.fill: parent
                         onPressed:  {   optionList.pressIndex=index  }
                         onClicked:   {   pLuxDomoListItem.setDeviceData(listData)   }
                     }
                 }
             }
    }

    ListView
    {
              id: list
              anchors.fill: parent
              clip: true
              model: model
              delegate:delegate
     }
}
