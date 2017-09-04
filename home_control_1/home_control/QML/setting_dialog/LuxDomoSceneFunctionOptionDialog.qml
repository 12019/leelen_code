import  QtQuick 1.0
import   LuxDomoSceneFunctionOption 1.0
import   "../content"
Rectangle
{
           id:scenefunctionOption
           objectName: "scenefunctionOption"
           width: 150
           height: 200
           color: "#f1f3e6"

           property int index: 0
           property string  sceneFile: ""
           property string  sceneName: ""
           property string  sceneImage: ""

          LuxDomoSceneFunctionOptionItem
          {
                id:pLuxDomoSceneFunctionOptionItem
                objectName: "pLuxDomoSceneFunctionOptionItem"
          }

          MouseArea{    anchors.fill: parent   }

          Rectangle
          {
                 id:title
                 objectName: "title"
                 width: scenefunctionOption.width
                 height:  scenefunctionOption.height*2/5
                 anchors.top:scenefunctionOption.top
                 anchors.left: scenefunctionOption.left
                 color: "#f1f3e6"
                 TextEdit
                  {
                      id:titleEdit
                      objectName: "titleEdit"
                      text: "功能选项"
                      anchors.verticalCenter:title.verticalCenter
                      anchors.horizontalCenter: title.horizontalCenter
                      font.pixelSize: 20
                      color: "black"
                  }
          }

          LuxDomoButton
          {
                 id:deleteButton
                 height: scenefunctionOption.height*3/10
                 width: scenefunctionOption.width/2
                 anchors.top:title.bottom
                 anchors.horizontalCenter: scenefunctionOption.horizontalCenter
                 name:"删除场景"
                 fontsize: 15
                 buttonColor:"red"
                 textcolor: "blue"
                 onClicked: {  pLuxDomoSceneFunctionOptionItem.deleteScene() }
           }

          LuxDomoButton
          {
                id:setName
                height: scenefunctionOption.height*3/10
                width: scenefunctionOption.width/2
                anchors.top:deleteButton.bottom
                anchors.horizontalCenter: scenefunctionOption.horizontalCenter
                name:"设置名称"
                fontsize: 15
                buttonColor:"red"
                textcolor: "blue"
                onClicked: {  pLuxDomoSceneFunctionOptionItem.editSceneName()  }
           }
}

