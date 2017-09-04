// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0
//import "stylesheet.js" as Stylesheet

Rectangle {
    id:accontrol
    width: 490
    height: 350
    color: "#00000000"
    radius: 10

    property string acBorImg : "images/home_control/device/kongtiao bg.png"


    BorderImage {
        id: acbg
        //source: "images/home_control/device/kongtiao bg.png"
        source:acBorImg
        anchors.fill: parent
    }
    MouseArea{
        anchors.fill: parent
    }

    LocalFontLoader{
        id:localfont
    }


    /*FontLoader{
        id:localfont
        source: "lib/fonts/fzlth.ttf"
    }*/

    TextEdit {
        font.family: localfont.name
        font.pixelSize:18
        text: m_pAccontrolDlg.label
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 300
        anchors.right: parent.right
        anchors.rightMargin: 100
        anchors.left: parent.left
        anchors.leftMargin: 100
        readOnly: true
        color:"#000000"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.top: parent.top
        anchors.topMargin: 15
    }

    DeviceButton {
        id: devicebutton1
        width: 200
        height: 55
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 60
        //空调开关
        onPreclicked: m_pAccontrolDlg.on_pushButtonACPower_clicked(m_pAccontrolDlg.stat)
        onNextclicked: m_pAccontrolDlg.on_pushButtonACPower_clicked(m_pAccontrolDlg.stat)
        msg:m_pAccontrolDlg.stat
    }

    DeviceButton {
        property bool isPowerOn:false
        id: devicebutton2
        width: 200
        height: 55
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 60

        //固定摆风
        onPreclicked:m_pAccontrolDlg.on_pushButtonACSwing_clicked("pre")
        onNextclicked: m_pAccontrolDlg.on_pushButtonACSwing_clicked("next")
        msg:m_pAccontrolDlg.swing
    }

    DeviceButton {
        id: devicebutton3
        //y: 177
        width: 200
        height: 55
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        //模式
        onPreclicked: m_pAccontrolDlg.on_pushButtonACMode_clicked("pre")
        onNextclicked: m_pAccontrolDlg.on_pushButtonACMode_clicked("next")
        msg:m_pAccontrolDlg.mode
    }

    DeviceButton {
        id: pushButtonACWind
        //x: 269
        //y: 177
        width: 200
        height: 55
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.verticalCenter: parent.verticalCenter
        //风速
        onPreclicked: m_pAccontrolDlg.on_pushButtonACWind_clicked("pre")
        onNextclicked: m_pAccontrolDlg.on_pushButtonACWind_clicked("next")
        msg:m_pAccontrolDlg.wind
    }

    DeviceButton {
        id: pushButtonACTemperature
        //y: 268
        width: 200
        height: 55
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 60
        //温度
        onPreclicked: m_pAccontrolDlg.on_pushButtonACTemperature_clicked("pre")
        onNextclicked: m_pAccontrolDlg.on_pushButtonACTemperature_clicked("next")
        msg:m_pAccontrolDlg.temp+"℃"
    }
}
