import   QtQuick 1.0
import   LuxDomoScene  1.0
import   "../content"
Rectangle
{
    id:scene
    objectName: "scene"
    width: 80
    height: 100
    color: "#00000000"

    property  string   type: ""
    property  string   sceneName: ""
    property  string   sceneImage: ""
    property  bool    isMoveParent: false
    property  int       oldY: 0

    LuxDomoSceneItem
    {
           id:pLuxDomoSceneItem
           objectName: "pLuxDomoSceneItem"
    }

    Item
    {
        width: scene.width
        height: scene.width
        anchors.top: scene.top
        anchors.left: scene.left
        Image
        {
            anchors.fill: parent
            source: scene.sceneImage
        }
     }

    LuxDomoLabel
    {
        width: scene.width
        height:  scene.height-scene.width
        anchors.bottom: scene.bottom
        textName: sceneName
    }

    MouseArea
    {
           id:mouseArea
           objectName: "mouseArea"
           anchors.fill: parent
           onClicked:
           {
               if(scene.isMoveParent==false)
               {
                   if(scene.type=="scene")
                    {   pLuxDomoSceneItem.useHomeScene()      }
                   else   if(scene.type=="room")
                    {  pLuxDomoSceneItem.showRoomControl()  }
               }
           }
           onPressed:{    pLuxDomoSceneItem.recordMouseY(mouseY) ; scene.oldY=mouseY   }
           onReleased:{   scene.isMoveParent=false   }
           onPositionChanged:
           {
               if(scene.oldY-mouseY>5||mouseY-scene.oldY>5)
                {
                   pLuxDomoSceneItem.moveParent(mouseY)
                   scene.isMoveParent=true
               }
           }
    }
}

