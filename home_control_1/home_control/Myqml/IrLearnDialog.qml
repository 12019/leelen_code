import QtQuick 1.0

Rectangle {
    id:irlearn
    width: 800
    height: 480
    color: "#ffffff"
    signal clicked


    property string irlearnzuocebg_1 : "images/home_control/list/zuocebg-1.png"
    property string irlearnzuocebg : "images/home_control/list/zuocebg.png"
    property string irlearnyixuexi : "images/home_control/list/yixuexi.png"
    property string irlearntanchukuang : "images/home_control/list/tanchukuang.png"
    property string quxiao: "images/home_control/list/quxiao.png"
    property string quxiao_1: "images/home_control/list/quxiao-1.png"


    LocalFontLoader{
        id:localfont
    }

    function setDefault(){
        irdeviceview.currentIndex = -1
    }

    MouseArea{
        anchors.fill: parent
    }

    function clearModel(){
        irDeviceModel.clear()
        irActionModel.clear()
        warning.visible = false
        irdeviceview.currentIndex = -1
    }

    function setModelPic(){
        irActionModel.setProperty(irActionview.currentIndex,"learn","true")
        warning.visible = false
    }

    function addModel(type,name,id){
        if(type == "music" | type == "ac" | type == "tv"){
            irDeviceModel.append({
                                 "name":name,
                                 "type":type,
                                 "deviceId":id
                                 })
        }
        else if(type == "irAction"){
            irActionModel.append({
                                 "name":name,
                                 "learn":""
                                 })
        }
    }

    Rectangle{
        color: "#efefef"
        anchors.left: irdeviceview.right
        anchors.leftMargin: -20
        anchors.top: ircancel.bottom
        anchors.topMargin: 0
        width: irActionview.width+40
        height: irActionview.height
    }


    ListModel {
        id: irDeviceModel
        ListElement {
            name: ""
            type:""
            deviceId:""
        }
    }

    Component {
        id: irDeviceDelegate
        Rectangle{
            id:irdeviceitem
            height: 147//irdeviceview.height/3
            width: irdeviceview.width
            color: "#00000000"
            Image {
                id:listimage
                /*source: irdeviceview.currentIndex==index? "images/home_control/list/zuocebg-1.png":
                                                     "images/home_control/list/zuocebg.png"*/
                source: irdeviceview.currentIndex==index? irlearnzuocebg_1:
                                                    irlearnzuocebg

                anchors.fill: parent
            }

            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    irdeviceview.currentIndex = index
                    warning.visible = false
                    //m_pIrLearnDialog.irTypeChose(index)
                }
                onPressed:
                {
                    warning.visible = false
                    //irdeviceview.currentIndex = index
                }

                onPressAndHold:
                {
                    //irdeviceview.currentIndex = index
                }
                onReleased: {
                    irActionModel.clear()
                    warning.visible = false
                    irdeviceview.currentIndex = index
                    m_pIrLearnDialog.irTypeChose(m_type.text,m_deviceid.text)
                }
                Text{
                    text: name
                    font.bold: true
                    width: irdeviceitem.width
                    height: irdeviceitem.height/4
                    anchors.bottom: parent.bottom
                    font.pixelSize: 20
                    font.family: localfont.name
                    color: "#000000"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                Text{
                    id:m_type
                    text: type
                    visible: false
                }
                Text{
                    id:m_deviceid
                    text: deviceId
                    visible: false
                }
                Rectangle{
                    width:irdeviceview.width-20
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
            scrollArea: irdeviceview
            height: irdeviceview.height
            width: 10
            x:irdeviceview.width-25
        }
        id: irdeviceview
        width: 255
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0

        anchors.top: ircancel.bottom
        anchors.topMargin: 0
        clip: true
        model: irDeviceModel
        delegate: irDeviceDelegate
        highlightMoveDuration: 0
        currentIndex: -1
    }


    ListModel {
        id: irActionModel
        ListElement {
            name: ""
            learn:""
        }
    }

    Component {
        id: irActionDelegate
        Rectangle{
            id:actionRec
            height: 100//irActionview.height/5
            width:irActionview.width
            color: "#00000000"
            Image {
                source: irlearnyixuexi//"images/home_control/list/yixuexi.png"
                anchors.verticalCenter: parent.verticalCenter
                x:500
                visible: learn == "true"?true:false
            }
            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    irActionview.currentIndex=index
                }
                onPressed:
                {
                    //warning.visible = false
                }

                onReleased: {
                    if(warning.visible){
                        warning.visible = false
                        return
                    }
                    irActionview.currentIndex=index
                    warning.visible = true
                    m_pIrLearnDialog.irLearn(index)
                }

                Text{
                    id:roomtext
                    text: name
                    font.bold: true
                    width: actionRec.width
                    height: actionRec.height
                    anchors.left: parent.left
                    font.pixelSize: 20
                    font.family: localfont.name
                    color: "#000000"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                Rectangle{
                    x:4
                    width:irActionview.width
                    height: 1
                    color: "#8f8f8f"
                    anchors.bottom: parent.bottom
                }
            }
        }
    }

    ListView
    {
        ScrollBar
        {
            scrollArea: irActionview
            height: irActionview.height
            width: 10
            x:irActionview.width-25
        }
        id: irActionview
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.left: irdeviceview.right
        anchors.leftMargin: 0
        anchors.top: ircancel.bottom
        anchors.topMargin: 0
        clip: true
        model: irActionModel
        delegate: irActionDelegate
        highlight: Rectangle{
            width: irActionview.width
            color: "#b4b4b4"
        }
        highlightMoveSpeed: 100000
    }

    Image {
        id: warning
        width: 310
        height: 220
        visible: false
        source: irlearntanchukuang//"images/home_control/list/tanchukuang.png"
        anchors.centerIn: parent
        Rectangle{
            width: 288
            height: 70
            color: "#0c9d7a"
            radius: 5
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: -1
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            Text {
                text: m_pIrLearnDialog.toast()
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.fill: parent
                font.pixelSize: 20
                font.family: localfont.name
                color: "#000000"
            }
           // anchors.b
        }
    }

    Rectangle {
        id: ircancel
        height: 39
        color: "#000000"
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        Button{
            height: 27
            width: 75
            color:"#00000000"
            anchors.left: parent.left
            anchors.leftMargin: 10
            image:quxiao//"images/home_control/list/quxiao.png"
            imagepressed: quxiao_1//"images/home_control/list/quxiao-1.png"
            anchors.verticalCenter: parent.verticalCenter
            textcolor: "#ffffff";
            fontsize: 18
            name: "     "+m_pHCSceneEdit.cancel
            onClicked: {
                clearModel()
                m_pIrLearnDialog.close()
            }
        }
    }
}
