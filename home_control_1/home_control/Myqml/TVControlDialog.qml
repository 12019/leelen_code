// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0

Rectangle {
    id:tv
    width: 490
    height: 270
    color: "#00000000"
    opacity: 1
    radius: 10

    BorderImage {
        id: lightbg
        source: "images/home_control/device/dianshi bg.png"
        anchors.fill: parent
    }

    LocalFontLoader{
        id:localfont
    }

    MouseArea{
        anchors.fill: parent
    }

    /******************电视窗口标签名称*****************/
    TextEdit {
        font.family: localfont.name
        font.pixelSize:18
        text: m_pTVControlDlg.label
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 210
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

    /*******************电视开关**********************/
    DeviceButton {
        id: pushButtonTVPower
        anchors.top: parent.top
        anchors.topMargin: 85
        anchors.left: parent.left
        anchors.leftMargin: 30
        onPreclicked:m_pTVControlDlg.on_pushButtonTVPower_clicked(m_pTVControlDlg.stat)
        onNextclicked:m_pTVControlDlg.on_pushButtonTVPower_clicked(m_pTVControlDlg.stat)
        msg:m_pTVControlDlg.stat
    }

    /******************电视模式*******************/
    DeviceButton {
        id: pushButtonTVMode
        anchors.top: parent.top
        anchors.topMargin: 85
        anchors.right: parent.right
        anchors.rightMargin: 30
        onPreclicked: m_pTVControlDlg.on_pushButtonTVMode_clicked()
        onNextclicked: m_pTVControlDlg.on_pushButtonTVMode_clicked()
        msg:m_pTVControlDlg.mode
    }

    /******************电视频道********************/
    DeviceButton {
        id: pushButtonTVChannel
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 30
        onPreclicked: m_pTVControlDlg.on_pushButtonTVChannel_clicked("channel_down")
        onNextclicked: m_pTVControlDlg.on_pushButtonTVChannel_clicked("channel_up")
        msg:"-"+m_pTVControlDlg.channel+"+"
    }

    /*****************电视音量******************/
    DeviceButton {
        id: pushButtonTVIVolume
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50
        anchors.right: parent.right
        anchors.rightMargin: 30
        onPreclicked: m_pTVControlDlg.on_pushButtonTVIVolume_clicked("vol_down")
        onNextclicked: m_pTVControlDlg.on_pushButtonTVIVolume_clicked("vol_up")
        msg:"-"+m_pTVControlDlg.vol+"+"
    }
}
