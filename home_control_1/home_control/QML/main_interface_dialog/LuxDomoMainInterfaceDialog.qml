import  QtQuick 1.0
import  LuxDomoMainInterface   1.0
import   "../content"
Rectangle
{
    id:mainInterface
    objectName: "mainInterface"
    width: 700
    height: 288
    color: "#00000000"

    property   int    controlH: 90
    property   int    controlW: 70
    property   int    pieceW: 130
    property   int    pieceH: 90
    property   int    leftMargin1: 50
    property  int     nameSize: 20

    property   bool    flag: false
    property   int       pressedX: 0
    property   string  usingSceneName: ""
    property   string  usingSceneFile: ""
    property   bool    hintHurdle: false

    function    clearroomModel()    {  roomModel.clear()   }
    function    addroomModel(roomUid,roomImage,roomName) {
                                        roomModel.append({ "roomUid":roomUid,
                                                                              "roomImage":roomImage,
                                                                              "roomName":roomName }) }
    function     changroomModel(index,propertyName,propertyValue){  roomModel.setProperty( index,propertyName,propertyValue) }
    function     removeroomModel(index){   roomModel.remove(index)  }

    function    clearsceneModel()    {  sceneModel.clear()   }
    function    addsceneModel(sceneFile,sceneImage,sceneName) {
                                        sceneModel.append({ "sceneFile":sceneFile,
                                                                              "sceneImage":sceneImage,
                                                                              "sceneName":sceneName }) }
    function     changsceneModel(index,propertyName,propertyValue){  sceneModel.setProperty( index,propertyName,propertyValue) }
    function     removesceneModel(index){   sceneModel.remove(index)  }

    MouseArea{  anchors.fill: parent  }

    LuxDomoMainInterfaceItem
    {
        id:pLuxDomoMainInterfaceItem
        objectName: "pLuxDomoMainInterfaceItem"
    }

    ListModel
    {
        id: roomModel
        ListElement
        {
            roomUid:""
            roomImage:""
            roomName:"11"
        }
    }

    ListModel
    {
        id: sceneModel
        ListElement
        {
            sceneFile:""
            sceneImage:""
            sceneName:""
        }
    }

    Component
     {
         id:roomDelegate
         Item
         {
             width: pieceW
             height: pieceH
             Column
             {
                 anchors.fill: parent
                 Image
                 {
                     width: controlW
                     height: controlW
                     source: roomImage
                     MouseArea
                     {
                         anchors.fill: parent
                         onClicked:
                         {
                             pLuxDomoMainInterfaceItem.showRoomControl(roomUid)
                             flag=true
                         }
                         onCanceled:
                         {
                             if(flag==false&&pressedX-mouseX<3&&pressedX-mouseX>-3)
                             {
                                 pLuxDomoMainInterfaceItem.showRoomControl(roomUid)
                             }
                             flag=false
                         }
                         onPressed:
                         {
                             pressedX=mouseX
                         }
                     }
                 }
                 LuxDomoLabel
                 {
                     width: controlW
                     height: controlH-controlW
                     textName: roomName
                     textSize: nameSize
                     textElide: true
                 }
             }
         }
     }

    Component
     {
         id:sceneDelegate
         Item
         {
             width: pieceW
             height: pieceH
             Column
             {
                 anchors.fill: parent
                 Image
                 {
                     width: controlW
                     height: controlW
                     source: sceneImage
                     MouseArea
                     {
                         anchors.fill: parent
                         onClicked:
                         {
                             usingSceneFile=sceneFile
                             usingSceneName=sceneName
                             hintHurdle=true
                             flag=true
                         }
                         onCanceled:
                         {
                             if(flag==false&&pressedX-mouseX<3&&pressedX-mouseX>-3)
                             {
                                 usingSceneFile=sceneFile
                                 usingSceneName=sceneName
                                 hintHurdle=true
                             }
                             flag=false
                         }
                         onPressed:
                         {
                             pressedX=mouseX
                         }
                     }
                 }
                 LuxDomoLabel
                 {
                     width: controlW
                     height: controlH-controlW
                     textName: sceneName
                     textSize: nameSize
                     textElide: true
                 }
             }
         }
     }

    Item
    {
        width: parent.width
        height: parent.height/2
        anchors.top: parent.top
        ListView
        {
            id: roomList
            width: 5*pieceW
            height: pieceH
            x:leftMargin1
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height/12
            clip: true
            orientation: ListView.Horizontal
            model: roomModel
            delegate:roomDelegate
         }
        LuxDomoLabel1
        {
            width: pieceW
            height: parent.height*11/12-pieceH
            x:leftMargin1
            anchors.top: parent.top
            textSize: 22
            textName: "房间"
        }
    }

    Item
    {
        width: parent.width
        height: parent.height/2
        anchors.bottom: parent.bottom
        ListView
        {
            id: sceneList
            width: 5*pieceW
            height: pieceH
            x:leftMargin1
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.height/10
            clip: true
            orientation: ListView.Horizontal
            model: sceneModel
            delegate:sceneDelegate
         }
        LuxDomoLabel1
        {
            width: pieceW
            height: parent.height*9/10-pieceH
            x:leftMargin1
            anchors.top: parent.top
            textSize: 22
            textName: "模式"
        }
    }

    Rectangle
    {
        width: parent.width-leftMargin1
        height: 1
        anchors.verticalCenter: parent.verticalCenter
        x:leftMargin1
        color: "#bfbfbf"
    }

    Rectangle
    {
        anchors.fill: parent
        visible: hintHurdle
        color:"#00000000"
        MouseArea
        {
            anchors.fill: parent
            onClicked:{  hintHurdle=false }
        }
        LuxDomoHintHurdle
        {
            width: parent.width/2
            height: parent.height*3/5
            anchors.horizontalCenter: parent.horizontalCenter
            textStr: "您确定启动 ' "+usingSceneName+" ' 吗?"
            imageStr:"../home_manage/irLearn_hint.png"
            onSureClicked: { pLuxDomoMainInterfaceItem.useHomeScene(usingSceneFile) ; hintHurdle=false  }
            onCancelClicked: { hintHurdle=false }
        }
    }
}
