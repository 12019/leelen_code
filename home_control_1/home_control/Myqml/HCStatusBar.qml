import QtQuick 1.0

Rectangle {
    id:statusbar1
    width: 800
    height: 50
    color:"#af000000"

    property string backS : "images/home_control/scene/back.png"
    property string backS_1 :  "images/home_control/scene/back-1.png"
    property string fangjianxueze : "images/home_control/menubar/fangjianxueze.png"
    property string fangjianxueze_1 : "images/home_control/menubar/fangjianxueze-1.png"
    property string hongwaixuexi : "images/home_control/menubar/hongwaixuexi.png"
    property string hongwaixuexi_1 : "images/home_control/menubar/hongwaixuexi-1.png"
    property string fanhui : "images/home_control/menubar/fanhui.png"
    property string fanhui_1 : "images/home_control/menubar/fanhui-1.png"
    property string down : "images/home_control/menubar/down.png"
    property string up :  "images/home_control/menubar/up.png"
    property bool isLabelValid: false
    property bool isBtnValid: false

    LocalFontLoader{
        id:localfont
    }

    function setHeightHide()
    {
        isLabelValid = false
        isBtnValid = false
        if(statusbar1.height == 50){
            action4.start()
        }
        else if(statusbar1.height == 158)
            action2.start()
    }

    function setHeightShow(){
        isLabelValid = true
        isBtnValid = false
        roomLabel.text = ""
        roomLabel.text = m_pHCStatusBar.lrName
        action3.start()
    }

    TextEdit{
        id:roomLabel
        width: 226
        height: 40
        color: "#ffffff"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        visible: isLabelValid//statusbar1.height == 50?true:false
        text:m_pHCStatusBar.lrName
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 18
        font.bold: true
        font.family: localfont.name
        cursorVisible: false
    }

    MouseArea{
        anchors.fill: parent
        onReleased: {
            m_pappWindow.setMainState("")
            if(statusbar1.height == 50){
                isLabelValid = false
                isBtnValid = true
                action.start()
            }
            else if(statusbar1.height == 158){
                isLabelValid = false
                isBtnValid = false
                action2.start()
            }
            else{
                isLabelValid = true
                isBtnValid = false
                action3.start()
            }
        }
    }

    Image{
        id:back
        width: 75
        height: 27
        source: backS//"images/home_control/scene/back.png"
        visible: isLabelValid//statusbar1.height == 50?true:false//m_bar
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
    }
    TextEdit {
        text: m_pHCStatusBar.exit
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.verticalCenter: parent.verticalCenter
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 18
        font.family: localfont.name
        color: "#ffffff"
        visible: isLabelValid
        font.bold: true
        cursorVisible: false
    }
    MouseArea{
        width: parent.width/2/3
        height: parent.height
        visible: back.visible
        onPressed: {
             back.source = backS_1//"images/home_control/scene/back-1.png"
        }
        onClicked: {
             m_pHCStatusBar.closeHomeControlDlg()
        }
        onReleased: {
             back.source = backS//"images/home_control/scene/back.png"
        }
    }


    Button{
        id:roomchose
        width: 80
        height: 80
        fontsize: 18
        visible:isBtnValid
        color: "#00000000"
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 175
        onClicked: m_pHCStatusBar.roomChose()
        image:fangjianxueze//"images/home_control/menubar/fangjianxueze.png"
        imagepressed: fangjianxueze_1//"images/home_control/menubar/fangjianxueze-1.png"
    }
    TextEdit {
        font.family: localfont.name
        font.pixelSize:18
        text: m_pHCStatusBar.hcselect
        anchors.horizontalCenter: roomchose.horizontalCenter
        anchors.top: roomchose.bottom
        anchors.topMargin: 0
        readOnly: true
        color:"#ffffff"
        visible: isBtnValid
        cursorVisible: false
    }

    Button{
        id:irLearn
        width: 80
        height: 80
        fontsize: 18
        visible: isBtnValid
        color: "#00000000"
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: m_pHCStatusBar.irLearn()
        image:hongwaixuexi//"images/home_control/menubar/hongwaixuexi.png"
        imagepressed: hongwaixuexi_1//"images/home_control/menubar/hongwaixuexi-1.png"
    }
    TextEdit {
        font.family: localfont.name
        font.pixelSize:18
        text: m_pHCStatusBar.irlearning
        anchors.horizontalCenter: irLearn.horizontalCenter
        anchors.top: irLearn.bottom
        anchors.topMargin: 0
        readOnly: true
        color:"#ffffff"
        visible: isBtnValid
        cursorVisible: false
    }

    Button{
        id:upbar
        width: 80
        height: 80
        fontsize: 18
        visible:isBtnValid
        color: "#00000000"
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 175
        image:fanhui//"images/home_control/menubar/fanhui.png"
        imagepressed: fanhui_1//"images/home_control/menubar/fanhui-1.png"
        onClicked: {
            /*if(m_pHCStatusBar.back == "设为主屏"){
                m_pHCStatusBar.setGround(true)
            }
            else{
                m_pHCStatusBar.setGround(false)
            }*/
            m_pHCStatusBar.setGround(false)
        }
    }
    TextEdit {
        font.family: localfont.name
        font.pixelSize:18
        text: m_pHCStatusBar.back
        anchors.horizontalCenter: upbar.horizontalCenter
        anchors.top: upbar.bottom
        anchors.topMargin: 0
        readOnly: true
        color:"#ffffff"
        visible: isBtnValid
        cursorVisible: false
    }
    Image {
        id: downarrow
        source: isBtnValid == false?
                     down:
                     up
        width: 30
        height: 14
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter
        visible: statusbar1.height > 10?true:false
    }

    PropertyAnimation{
        id:action
        target: statusbar1
        property: "height"
        from:10
        to:158
        duration: 200
    }
    PropertyAnimation{
        id:action2
        target: statusbar1
        property: "height"
        from:158
        to:10
        duration: 200
    }

    PropertyAnimation{
        id:action3
        target: statusbar1
        property: "height"
        from:10
        to:50
        duration: 200
        onCompleted: {
            console.log("action3")
        }
    }
    PropertyAnimation{
        id:action4
        target: statusbar1
        property: "height"
        from:50
        to:10
        duration: 200
    }
}
