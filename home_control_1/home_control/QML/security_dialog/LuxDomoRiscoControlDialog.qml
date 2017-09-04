import  QtQuick 1.0
import  "../content"
import  LuxDomoRiscoControl  1.0
Rectangle
{
    id:control
    objectName: "control"
    width: 896
    height: 600
    color:"#efeff4"

    MouseArea{  anchors.fill: parent  }

    LuxDomoRiscoControlItem
    {
        id:pLuxDomoRiscoControlItem
        objectName: "pLuxDomoRiscoControlItem"
    }

   Component.onCompleted: {  pLuxDomoRiscoControlItem.init()   }

   function    clearModel()     {   model.clear()   }
   function    addModel(listData,activate){  model.append({ "listData":listData,"activate":activate})  }
   function    changModel(index,propertyName,propertyValue){  model.setProperty( index,propertyName,propertyValue) }

    ListModel
    {
        id: model
        ListElement
        {
            listData:"NULL"
            activate:false
        }
    }

    Component
     {
         id:delegate
         Rectangle
         {
             width: list.width
             height: list.height/6
             Rectangle
             {
                 width: parent.width
                 height: 1
                 anchors.right:parent.right
                 anchors.bottom: parent.bottom
                 color: "#bfbfbf"
             }
             LuxDomoLabel1
             {
                 width: parent.width*3/4
                 height: parent.height
                 margin: parent.width/20
                 anchors.left: parent.left
                 textName: listData
                 textSize: 23
             }
             Item
             {
                 width: parent.width/4
                 height: parent.height
                 anchors.right: parent.right
                 LuxDomoButton
                 {
                     width: parent.width/2
                     height: parent.height/2
                     anchors.verticalCenter: parent.verticalCenter
                     anchors.horizontalCenter: parent.horizontalCenter
                     image:activate?"../security/risco_activate.png":"../security/risco_not_activate.png"
                     imagepressed: activate?"../security/risco_activate.png":"../security/risco_not_activate.png"
                     name:activate?"激活":"未激活"
                     fontsize: 20
                     onClicked:
                     {
                         if(activate)
                         {
                             pLuxDomoRiscoControlItem.activateControl(index,false)
                         }
                         else
                         {
                             pLuxDomoRiscoControlItem.activateControl(index,true)
                         }
                     }
                 }
             }
         }
     }

    ListView
    {
        id: list
        width: parent.width
        height: parent.height
        anchors.top: parent.top
        clip: true
        model: model
        delegate:delegate
     }
}

