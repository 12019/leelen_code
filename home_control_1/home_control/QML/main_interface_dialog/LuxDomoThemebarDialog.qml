import  QtQuick 1.0
import   "../content"
import   LuxDomoThemebar 1.0
Rectangle
{
     id:themebar
     objectName: "themebar"
     width: 800
     height: 240

     LuxDomoThemebarItem
     {
         id:pLuxDomoThemebarItem
         objectName: "pLuxDomoThemebarItem"
     }

     Image
     {
         anchors.fill: parent
         source: "../main_interface/theme_background.png"
     }

     function    clearModel()    {  model.clear()   }
     function    addModel(roomName,tempValue,humidnessValue,airValue) {
                                         model.append({ "roomName":roomName,
                                                                               "tempValue":tempValue,
                                                                               "humidnessValue":humidnessValue,
                                                                               "airValue":airValue }) }
     function     changModel(index,propertyName,propertyValue){  model.setProperty( index,propertyName,propertyValue) }
     function     removeModel(index){   model.remove(index)  }

     ListModel
     {
         id: model
         ListElement
         {
             roomName:"无房间"
             tempValue:"离线"
             humidnessValue:"离线"
             airValue:"离线"
         }
     }

     Component
     {
         id:delegate
         Item
         {
             width: list.width
             height: list.height
             Image
             {
                 anchors.fill: parent
                 source: "../main_interface/tem_background.png"
             }
             LuxDomoLabel
             {
                 width: parent.width
                 height: parent.height/4
                 anchors.top: parent.top
                 textName: roomName
                 textSize: 20
                 textElide: true
                 totalLength: 8
                 showLength: 6
             }
             Column
             {
                 width: parent.width
                 height: parent.height*3/4
                 anchors.bottom: parent.bottom
                 Item
                 {
                     width: parent.width
                     height: parent.height/3
                     Row
                     {
                         anchors.fill: parent
                         Item
                         {
                             width: parent.width*2/5
                             height: parent.height
                             Image
                             {
                                 width: parent.width*3/5
                                 height: parent.height*3/5
                                 anchors.verticalCenter: parent.verticalCenter
                                 anchors.horizontalCenter: parent.horizontalCenter
                                 source: "../main_interface/temp_value.png"
                             }
                         }
                         Item
                         {
                             width: parent.width*3/5
                             height: parent.height
                             LuxDomoLabel
                             {
                                 anchors.fill: parent
                                 textName: tempValue
                                 textSize: 18
                             }
                         }
                     }
                 }
                 Item
                 {
                     width: parent.width
                     height: parent.height/3
                     Row
                     {
                         anchors.fill: parent
                         Item
                         {
                             width: parent.width*2/5
                             height: parent.height
                             Image
                             {
                                 width: parent.width*3/5
                                 height: parent.height*3/5
                                 anchors.verticalCenter: parent.verticalCenter
                                 anchors.horizontalCenter: parent.horizontalCenter
                                 source: "../main_interface/humidness_value.png"
                             }
                         }
                         Item
                         {
                             width: parent.width*3/5
                             height: parent.height
                             LuxDomoLabel
                             {
                                 anchors.fill: parent
                                 textName: humidnessValue
                                 textSize: 18
                             }
                         }
                     }
                 }
                 Item
                 {
                     width: parent.width
                     height: parent.height/3
                     Row
                     {
                         anchors.fill: parent
                         Item
                         {
                             width: parent.width*2/5
                             height: parent.height
                             Image
                             {
                                 width: parent.width*3/5
                                 height: parent.height*3/5
                                 anchors.verticalCenter: parent.verticalCenter
                                 anchors.horizontalCenter: parent.horizontalCenter
                                 source: "../main_interface/air_value.png"
                             }
                         }
                         Item
                         {
                             width: parent.width*3/5
                             height: parent.height
                             LuxDomoLabel
                             {
                                 anchors.fill: parent
                                 textName: airValue
                                 textSize: 18
                             }
                         }
                     }
                 }
             }
         }
     }

     ListView
     {
         id:list
         width: parent.width/5
         height: parent.height*3/4
         anchors.bottom: parent.bottom
         anchors.right: parent.right
         anchors.rightMargin: parent.width/18
         clip: true
         orientation: ListView.Horizontal
         model: model
         delegate:delegate
      }
}
