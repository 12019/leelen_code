import  QtQuick 1.0
import  LuxDomoSceneToolbar  1.0
import   "../content"
Rectangle
{
    id:sceneToolbar
    objectName: "sceneToolbar"
    width: 450
    height: 480
    color:"#efeff4"

    function    clearModel()      {   model.clear()    }
    function    addModel(sceneImage,sceneName,sceneFileName,isEnable)
    {
        model.append({ "sceneImage":sceneImage,
                                     "sceneName":sceneName,
                                     "sceneFileName":sceneFileName,
                                     "isEnable":isEnable   })
    }
    function    changModel(index,propertyName,propertyValue){  model.setProperty( index,propertyName,propertyValue) }

    function    init(){  pLuxDomoSceneToolbarItem.init()  }

    MouseArea{ anchors.fill: parent   }

    LuxDomoSceneToolbarItem
    {
          id:pLuxDomoSceneToolbarItem
          objectName: "pLuxDomoSceneToolbarItem"
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

    ListModel
    {
        id: model
        ListElement {   sceneImage:"";   sceneName:"" ; sceneFileName:"" ; isEnable:false }
     }

    Component
     {
         id:delegate
         Item
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
             Row
             {
                 anchors.fill: parent
                 Item
                 {
                      width: parent.width/6
                      height: parent.height
                      Image
                      {
                         width: parent.height*3/5
                         height: parent.height*3/5
                         anchors.verticalCenter: parent.verticalCenter
                         anchors.horizontalCenter: parent.horizontalCenter
                         source: sceneImage
                      }
                 }
                 Item
                 {
                     width: parent.width*2/3
                     height: parent.height
                     LuxDomoLabel
                     {
                         width: parent.width/2
                         height: parent.height
                         anchors.left: parent.left
                         textName: sceneName
                         textSize: 20
                     }
                     MouseArea
                     {
                         anchors.fill: parent
                         onClicked: {   pLuxDomoSceneToolbarItem.showHomeScene(sceneFileName,sceneName)  }
                     }
                 }
                 Item
                 {
                     width: parent.width/6
                     height: parent.height
                     Image
                     {
                         width: parent.width*4/5
                         height: parent.height*2/5
                         anchors.verticalCenter: parent.verticalCenter
                         anchors.horizontalCenter: parent.horizontalCenter
                         source: isEnable?"../setting/on_off_on.png":"../setting/on_off_off.png"
                     }
                     MouseArea
                     {
                         anchors.fill: parent
                         onClicked:
                         {
                             if(isEnable)
                             {
                                 pLuxDomoSceneToolbarItem.enableHomeScene(sceneFileName,"disenable",index)
                             }
                             else
                             {
                                 pLuxDomoSceneToolbarItem.enableHomeScene(sceneFileName,"enable",index)
                             }
                          }
                     }
                 }
             }
         }
     }

    ListView
    {
        id: list
        width: parent.width*9/10
        height: parent.height*5/6
        y:parent.height/6
        x:parent.width/20
        clip: true
        model: model
        delegate: delegate
     }
}

