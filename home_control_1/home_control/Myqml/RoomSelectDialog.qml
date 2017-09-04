import QtQuick 1.0

Rectangle {
    id:roomselectdlg
    objectName: "roomselect"
    width: 800
    height: 480
    opacity: 1
    color:"#ffffff"
    visible: true

    property string quxiao: "images/home_control/list/quxiao.png"
    property string quxiao_1: "images/home_control/list/quxiao-1.png"

    MouseArea{
        anchors.fill: parent
    }

    Timer {
        id:timer
        interval: 1000
        running: false;
        repeat: false
        onTriggered:{
            m_pHCRoomSelect.roomList(roomlistview.currentIndex)
            //m_pHCRoomSelect.loadRoomMap()
        }
    }

    LocalFontLoader{
        id:localfont
    }

    function loadLayer(){
        m_pHCRoomSelect.loadLayerlist()
        layerlistview.currentIndex=m_pHCRoomSelect.layerId
        roomlistview.currentIndex=m_pHCRoomSelect.roomId
    }

    function clearModel(type){
        if(type == "lclear"){
            layerModel.clear()
        }
        else if(type == "rclear"){
            roomModel.clear()
        }
        else if(type == "lrclear"){
            layerModel.clear()
            roomModel.clear()
        }
    }

    function addModel(type,name){
        if(type == "layer"){
            layerModel.append({"name": name})
        }
        else if(type == "room")
            roomModel.append({"name":name})
    }

    function addRoomModel(name,image){
        console.log(name,image)
        roomModel.append({"name":name,
                         "image":image})
    }

    Rectangle{
        color: "#efefef"
        anchors.left: layerlistview.right
        anchors.leftMargin: -20
        anchors.top: cancel.bottom
        anchors.topMargin: 0
        width: roomlistview.width+40
        height: roomlistview.height
    }

    ListModel {
        id: layerModel
        ListElement {
            name: ""
        }
    }

    Component {
        id: layerDelegate
        Rectangle{
            id:layerRev
            height: 147//layerlistview.height/3
            width:layerlistview.width
            color: "#00000000"
            Image {
                source: layerlistview.currentIndex==index? "images/home_control/list/zuocebg-1.png":
                                                     "images/home_control/list/zuocebg.png"
                anchors.fill: parent
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    layerlistview.currentIndex=index
                    m_pHCRoomSelect.layerList(name)
                }
                onPressed:
                {
                    console.log("pressed")
                    layerlistview.flag = true
                    //layerlistview.currentIndex=index
                    //m_pHCRoomSelect.layerList(index)
                    //mylistview.currentIndex=index;
                }

                onPressAndHold:
                {
                    //layerlistview.currentIndex=index
                }

                onCanceled: {
                    console.log("cancel")
                    if(layerlistview.flag){
                        layerlistview.currentIndex=index
                        m_pHCRoomSelect.layerList(name)
                    }
                }

                Text{
                    id:layertext
                    text: name
                    font.bold: true
                    width: layerRev.width
                    height: layerRev.height/4
                    anchors.bottom: parent.bottom
                    font.pixelSize: 20
                    font.family: localfont.name
                    color: "#000000"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                Rectangle{
                    width:layerlistview.width-20
                    height: 1
                    color: "#9f9f9f"
                    anchors.bottom: parent.bottom
                }
            }
        }
    }

    ListView
    {
        ScrollBar
        {
            scrollArea: layerlistview
            height: layerlistview.height
            width: 10
            x:layerlistview.width-25
        }
        property bool flag: true
        id: layerlistview
        width:255
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: cancel.bottom
        anchors.topMargin: 0
        clip: true
        model: layerModel
        delegate: layerDelegate
        currentIndex: -1
        //currentIndex: m_pHCRoomSelect.layerId
        onMovementStarted: {
            console.log("move start")
            layerlistview.flag = false
        }
    }

    /*ListModel {
        id: roomModel
        ListElement {
            name: ""
        }
    }

    Component {
        id: roomDelegate
        Rectangle{
            id:roomRec
            height: 100//roomlistview.height/5
            width:roomlistview.width
            color: "#00000000"
            Image {
                id: roombg
                source: "images/home_control/list/keting.png"
                anchors.verticalCenter: parent.verticalCenter
                x:10
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    m_pappWindow.setloading(true)
                    roomlistview.currentIndex=index

                    timer.start()
                }
                onPressed:
                {
                    //roomlistview.currentIndex=index;
                }

                onPressAndHold:
                {
                    roomlistview.currentIndex=index
                }
                Text{
                    id:roomtext
                    text: name
                    font.bold: true
                    width: roomRec.width
                    height: roomRec.height
                    anchors.left: parent.left
                    font.pixelSize: 20
                    font.family: localfont.name
                    color: "#000000"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                Rectangle{
                    x:4
                    width:roomlistview.width
                    height: 1
                    color: "#8f8f8f"
                    anchors.bottom: parent.bottom
                }
            }
        }
    }

    ListView
    {
        id: roomlistview
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: layerlistview.right
        anchors.leftMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: cancel.bottom
        anchors.topMargin: 0
        clip: true
        model: roomModel
        delegate: roomDelegate
        highlight: Rectangle{
            width: roomlistview.width
            color: "#b4b4b4"
        }
        highlightMoveSpeed: 100000
        currentIndex: m_pHCRoomSelect.roomId
    }*/
    ListModel {
            id: roomModel
            ListElement {
                name: ""
                image: ""
            }
        }

    Component {
        id: roomDelegate
        Item{
            id:roomRec
            height: roomlistview.height/2-10//roomlistview.height/5
            width:roomlistview.width/2-10
            //color: "#ffffff"
            Image {
                id: roombg
                source: image
                anchors.fill: parent
                x:10
                anchors.rightMargin: 2
                anchors.leftMargin: 2
                anchors.bottomMargin: 30
                anchors.topMargin: 2
            }
            Rectangle {
                id: rec
                anchors.fill: parent
                x:10
                anchors.rightMargin: 2
                anchors.leftMargin: 2
                anchors.bottomMargin: 30
                anchors.topMargin: 2
                color: roomlistview.currentIndex==index?"#3f000000":"#00000000"
            }
            MouseArea
            {
                id:mousearea
                anchors.fill: parent
                onClicked:
                {
                    m_pappWindow.setloading(true)
                    roomlistview.currentIndex=index
                    timer.start()
                }
                onPressed:
                {
                    roomlistview.flag = true
                    //roomlistview.currentIndex=index;
                }

                onPressAndHold:
                {
                    roomlistview.currentIndex=index
                }

                onCanceled: {
                    if(roomlistview.flag){
                        m_pappWindow.setloading(true)
                        roomlistview.currentIndex=index
                        timer.start()
                    }
                }

                Text{
                    id:roomtext
                    text: name
                    font.bold: true
                    width: roomRec.width
                    //height: roomRec.height
                    anchors.left: parent.left
                    font.pixelSize: 20
                    font.family: localfont.name
                    color: "#000000"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    anchors.bottom: parent.bottom
                }
            }
        }
    }

    GridView
    {
        ScrollBar
        {
            scrollArea: roomlistview
            height: roomlistview.height
            width: 10
            x:roomlistview.width-25
        }
        property bool flag: true
        highlightMoveDuration: 0
        id: roomlistview
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: layerlistview.right
        anchors.leftMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: cancel.bottom
        anchors.topMargin: 0
        clip: true
        model: roomModel
        delegate: roomDelegate
        cellWidth: roomlistview.width/2-10
        cellHeight: roomlistview.height/2-10
        onMovementStarted: {
            console.log("move start")
            roomlistview.flag = false
        }
    }



    Rectangle {
        id: cancel
        height: 39
        color: "#000000"
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        Image {
            id: cancelImg
            source: quxiao
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
        }
        TextEdit{
            text: m_pHCRoomSelect.cancel
            anchors.left: cancelImg.right
            anchors.leftMargin: -20
            anchors.verticalCenter: parent.verticalCenter
            font.family: localfont.name
            font.pixelSize: 18
            color: "#ffffff"
        }
        MouseArea{
            width: parent.width/2/3
            height: parent.height
            onPressed: {
                cancelImg.source = quxiao_1
            }
            onReleased: {
                cancelImg.source = quxiao
                m_pHCRoomSelect.close()
            }
        }
    }
}
