import QtQuick 1.0
import "../content"
Item {
    id: developer
    width: 800
    height: 480
    objectName: "Developer"

    property real mFontSize: 20
    property int mheight: 50
    property string layerList
    property string roomList
    property int count
    property string layerName: m_pParaStyle.getLabelText("layer")
    property string roomName: m_pParaStyle.getLabelText("room")
    property string deviceInfo



    LocalFontLoader{
        id:localfont
    }

    function adaptInterface(){
        mFontSize = mFontSize*m_pappWindow.screenPerInch()
        mheight = mheight*m_pappWindow.screenPerInch()
        devInfo.adaptInterface()
        devModel.clear()
        //devInfo.mFontSize = devInfo.mFontSize*m_pappWindow.screenPerInch()
    }

    function setView(layername){
        layerModel.append({"layername": layername})
    }

    function setRoomView(roomName,roomId){
        roomModel.append({"roomname":roomName,
                                              "roomid":roomId})
    }

    function setDevView(_name,_id,_type){
        devModel.append({"_name": _name,
                                            "_id":_id,
                                            "_type":_type
                                            })
    }

    Button{
        id:layer
        height: mheight
        width: parent.width/2
        color: "#979595"
        anchors.right: parent.right
        anchors.rightMargin: parent.width/2
        anchors.left: parent.left
        anchors.leftMargin: 0
        border.width: 3
        fontsize: mFontSize
        border.color:layer.color
        onPressed: {
            border.color = "#000000"
        }
        onReleased: {
            border.color = layer.color
        }

        onClicked: {
            layerModel.clear()
            developer.state = "ShowLayerView"
            layerList = m_pDeveloper.layerList()
            var _layerList = layerList.split(";")
            //layerList = layerList.split(";")
            count = _layerList.length
            console.log(_layerList)
            for(var i = 1;i<count;i++){
                setView(String(_layerList.slice(i,i+1)))
            }
            console.log(name)
        }
        name: layerName//m_pParaStyle.getLabelText("layer")
    }

    Button{
        id:room
        height: mheight
        width: parent.width/2
        color: "#979595"
        anchors.right: parent.right
        anchors.rightMargin: 0
        border.width: 3
        fontsize: mFontSize
        border.color:layer.color
        onPressed: {
            border.color = "#000000"
        }
        onReleased: {
            border.color = layer.color
        }

        onClicked: {
            roomModel.clear()
            developer.state = "ShowRoomView"
            roomList = m_pDeveloper.roomList()
            var _roomList = roomList.split("&")
            var roomName = String(_roomList.slice(0,1))
            var roomId = String(_roomList.slice(1,2))

            //var length = roomName.length
            roomName = roomName.split(";")
            roomId = roomId.split(";")
            var length = roomName.length
            for(var i=1;i<length;i++){
                setRoomView(String(roomName.slice(i,i+1)),String(roomId.slice(i,i+1)))
            }
        }
        name:roomName
    }




    ListModel {
        id: devModel
        ListElement {
            _name: ""
            _type: ""
            _id:""
        }
    }

    Component {
        id: devDelegate

       Item{
            id:devItem
            height: devListView.cellHeight
            width:devListView.cellWidth
            Rectangle{
                height: devListView.cellHeight// - 2
                width:devListView.cellWidth// - 2
                border.width: 2
                border.color: "#000000"
                color: devListView.currentIndex == index? "#a0a0a0":"#ffffff"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    id: name
                    text: _name
                    font.family: localfont.name
                    font.pixelSize: mFontSize
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    id: idid
                    text: _id
                    visible: false
                }
                Text {
                    id: type
                    text: _type
                    visible: false
                }
                MouseArea{
                    id:devMouse
                    anchors.fill: parent
                    onClicked: {
                        if(developer.state == "ShowDevInfo"){
                            developer.state = ""
                            return
                        }
                        devListView.currentIndex = index
                        developer.state = "ShowDevInfo"
                        console.log(idid.text,type.text,name.text)
                        deviceInfo = m_pDeveloper.deviceInfo(idid.text,type.text)
                        var Info = deviceInfo.split(";")
                        console.log(Info)
                        devInfo.setDevInfo(String(Info.slice(0,1)),String(Info.slice(1,2)),String(Info.slice(2,3)),String(Info.slice(3,4)))
                    }
                }
            }
        }
    }

    GridView
    {
        id: devListView
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top:layer.bottom
        anchors.topMargin: 0
        clip: true
        model: devModel
        delegate: devDelegate
        cellWidth: devListView.width/5
        cellHeight: cellWidth
        currentIndex: -1
        onClipChanged: {
            devModel.clear()
        }
    }


    ListModel {
        id: layerModel
        ListElement {
            layername:""
        }
    }
    Component {
        id: layerDelegate
        Rectangle{
           width: layerView.width
           height: mheight
           Text {
               id: name
               text: layername
               font.family: localfont.name
               font.pixelSize: mFontSize
               anchors.horizontalCenter: parent.horizontalCenter
               anchors.verticalCenter: parent.verticalCenter
           }
           Rectangle{
               width:layerView.width
               height: 1
               color: "#000000"
               anchors.bottom: parent.bottom
           }
           Rectangle{
               anchors.fill: parent
               color: layerviewmouse.pressed?"#5f000000":"#00000000"
           }

           MouseArea{
               id:layerviewmouse
               anchors.fill: parent
               onClicked: {
                   layerName  = m_pParaStyle.getLabelText("layer") + ":" + name.text
                   m_pDeveloper.setLayer(name.text)
                   developer.state = ""
               }
           }
        }
    }
    ListView {
        id: layerView
        width: parent.width/2
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: mheight
        clip: true
        delegate: layerDelegate
        model: layerModel
        x:parent.width
        onClipChanged: {
            layerModel.clear()
        }
    }




    ListModel {
        id: roomModel
        ListElement {
            roomname:""
            roomid:""
        }
    }
    Component {
        id: roomDelegate
        Rectangle{
           width: roomView.width
           height: mheight
           Text {
               id: _roomname
               text: roomname
               font.family: localfont.name
               font.pixelSize: mFontSize
               anchors.horizontalCenter: parent.horizontalCenter
               anchors.verticalCenter: parent.verticalCenter
           }
           Text {
               id: _id
               text: roomid
               font.family: localfont.name
               visible: false
           }
           Rectangle{
               width:roomView.width
               height: 1
               color: "#000000"
               anchors.bottom: parent.bottom
           }
           Rectangle{
               anchors.fill: parent
               color: roomviewmouse.pressed?"#5f000000":"#00000000"
           }

           MouseArea{
               id:roomviewmouse
               anchors.fill: parent
               onClicked: {
                   roomName  = m_pParaStyle.getLabelText("room") + ":" + _roomname.text
                   m_pDeveloper.setRoom(_id.text)
                   devModel.clear()
                   developer.state = ""
               }
           }
        }
    }

    ListView {
        id: roomView
        width: parent.width/2
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: mheight
        clip: true
        delegate: roomDelegate
        model: roomModel
        x:parent.width
        onClipChanged: {
            roomModel.clear()
        }
    }

    DeviceInfo{
        id:devInfo
        //anchors.fill: parent
        y:parent.height
        //anchors.horizontalCenter: parent.horizontalCenter
    }






    states: [
        State {
            name: "ShowLayerView"
            PropertyChanges {
                target: layerView
                x:0
            }
        },
        State {
            name: "ShowRoomView"
            PropertyChanges {
                target: roomView
                x:room.x
            }
        },
        State {
            name: "ShowDevInfo"
            PropertyChanges {
                target: devInfo
                y:0
            }
        }
    ]
    transitions: Transition {
        NumberAnimation{ properties: "x,y";easing.type: Easing.OutBounce;duration:1000}
    }
}

