// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0


Rectangle {
    id:curtain
    width: 372
    height: 206
    color: "#00000000"
    opacity: 1
    radius: 10

    LocalFontLoader{
        id:localfont
    }


    property string curtainBorderImg : "images/home_control/device/chuanglian bg.png"
    property string curtainKai : "images/home_control/device/kai.png"
    property string curtainKai_1 : "images/home_control/device/kai-1.png"
    property string curtainGuan : "images/home_control/device/guan.png"
    property string curtainGuan_1 : "images/home_control/device/guan-1.png"
    property string curtainTing : "images/home_control/device/ting.png"
    property string curtainTing_1 : "images/home_control/device/ting-1.png"

    BorderImage {
        id: lightbg
        //source: "images/home_control/device/chuanglian bg.png"
        source: curtainBorderImg
        anchors.fill: parent
    }

    MouseArea{
        anchors.fill: parent
    }

    Button {
        id: pushButtonCurtainOpen
        x: 27
        y: 76
        width: 50
        height: 55
        color:"#00000000"
        //image:"images/home_control/device/kai.png"
        //imagepressed: "images/home_control/device/kai-1.png"
        image:curtainKai
        imagepressed: curtainKai_1
        onClicked: m_pCurtainControlDlg.on_pushButtonCurtainOpen_clicked()
    }

    Button {
        id: pushButtonCurtainStop
        x: 161
        y: 76
        width: 50
        height: 55
        color:"#00000000"
        //image:"images/home_control/device/guan.png"
        //imagepressed: "images/home_control/device/guan-1.png"

        image:curtainGuan
        imagepressed: curtainGuan_1

        onClicked: m_pCurtainControlDlg.on_pushButtonCurtainStop_clicked()
    }

    Button {
        id: pushButtonCurtainClose
        x: 299
        y: 76
        width: 50
        height: 55
        color:"#00000000"
        //image:"images/home_control/device/ting.png"
        //imagepressed: "images/home_control/device/ting-1.png"

        image:curtainTing
        imagepressed: curtainTing_1
        onClicked: m_pCurtainControlDlg.on_pushButtonCurtainClose_clicked()
    }

    TextEdit {
        font.family: localfont.name
        font.pixelSize:18
        text: m_pCurtainControlDlg.label
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 150
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 50
        readOnly: true
        color:"#000000"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.top: parent.top
        anchors.topMargin: 15
    }
}
