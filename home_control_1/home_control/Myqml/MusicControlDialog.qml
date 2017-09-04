// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0

Rectangle
{
    id:music
    width: 490
    height: 326
    color: "#00000000"
    opacity: 1

    LocalFontLoader{
        id:localfont
    }

    function resetState(){
        console.log(pushButtonBMSource.msg)
        pushButtonBMSource.focus = true
    }

    BorderImage {
        id: musicbg
        source: "images/home_control/device/yinyue bg.png"
        anchors.fill: parent
    }
    MouseArea{
        anchors.fill: parent
    }

    TextEdit {
        id:label
        font.family: localfont.name
        font.pixelSize:18
        text: m_pMusicControlDlg.label1//"音乐控制"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 280
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

    TextEdit {
        x: 179
        y: 67
        width: 95
        height: 29
        font.family: localfont.name
        font.pixelSize:18
        text: m_pMusicControlDlg.label2//"声音模式:"
        anchors.right: unmute.left
        anchors.rightMargin: 6
        anchors.left: parent.left
        anchors.leftMargin: 179
        readOnly: true
        color:"#000000"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.top: parent.top
        anchors.topMargin: 65
    }

    Button{//非静音
        id:unmute
        width: 26
        height: 22
        color: "#00000000"
        anchors.left: parent.left
        anchors.leftMargin: 280
        anchors.top: parent.top
        anchors.topMargin: 67
        image:"images/home_control/device/shengyin.png"
        imagepressed: "images/home_control/device/jingyin.png"
        onClicked: {
            if(!m_pMusicControlDlg.musicPowerStat())
                return
            m_pMusicControlDlg.on_pushButtonBMMute_clicked()
            //mute.visible=true
            //unmute.visible=false
            m_pMusicControlDlg.mute = true
        }
        visible: !m_pMusicControlDlg.mute
    }

    Button{//静音
        id:mute
        width: 26
        height: 22
        color: "#00000000"
        anchors.left: parent.left
        anchors.leftMargin: 280
        anchors.top: parent.top
        anchors.topMargin: 67
        image:"images/home_control/device/jingyin.png"
        imagepressed: "images/home_control/device/shengyin.png"
        //visible: false
        onClicked: {
            if(!m_pMusicControlDlg.musicPowerStat())
                return
            m_pMusicControlDlg.on_pushButtonBMUnmute_clicked()
            //mute.visible=false
            //unmute.visible=true
            m_pMusicControlDlg.mute = false
        }
        visible: m_pMusicControlDlg.mute
    }

    DeviceButton { //资源
        id: pushButtonBMSource
        anchors.left: parent.left
        anchors.leftMargin: 35
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        onPreclicked: m_pMusicControlDlg.on_pushButtonBMSource_clicked("pre")
        onNextclicked: m_pMusicControlDlg.on_pushButtonBMSource_clicked("next")
        msg:m_pMusicControlDlg.source
    }

    DeviceButton {//音量
        id: devicebutton2
        anchors.right: parent.right
        anchors.rightMargin: 35
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        onPreclicked: m_pMusicControlDlg.on_pushButtonBMVolume_clicked("vol_down")
        onNextclicked: m_pMusicControlDlg.on_pushButtonBMVolume_clicked("vol_up")
        msg:"-"+m_pMusicControlDlg.vol+"+"
    }
    DeviceButton {//电源
        id: power
        anchors.top: pushButtonBMPlay.bottom
        anchors.topMargin: 5
        anchors.horizontalCenter: parent.horizontalCenter
        msg:m_pMusicControlDlg.power
        onPreclicked: {
            m_pMusicControlDlg.on_pushButtonBMPower_clicked("")
            /*if(m_pMusicControlDlg.power == "开"){
                m_pMusicControlDlg.on_pushButtonBMPower_clicked("poweroff")
                m_pMusicControlDlg.power = "关"
            }
            else{
                m_pMusicControlDlg.on_pushButtonBMPower_clicked("poweron")
                m_pMusicControlDlg.power = "开"
            }*/
        }
        onNextclicked: {
            m_pMusicControlDlg.on_pushButtonBMPower_clicked("")
            /*if(m_pMusicControlDlg.power == "开"){
                m_pMusicControlDlg.on_pushButtonBMPower_clicked("poweroff")
                m_pMusicControlDlg.power = "关"
            }
            else{
                m_pMusicControlDlg.on_pushButtonBMPower_clicked("poweron")
                m_pMusicControlDlg.power = "开"
            }*/
        }
    }


    Button {
        id: pushButtonBMPrevMusic
        width: 39
        height: 22
        color: "#00000000"
        anchors.left: parent.left
        anchors.leftMargin: 100
        anchors.verticalCenter: parent.verticalCenter
        image:"images/home_control/device/shangyishou.png"
        imagepressed: "images/home_control/device/shangyishou-1.png"
        onClicked: m_pMusicControlDlg.on_pushButtonBMPrevMusic_clicked()
    }

    Button {//播放
        id: pushButtonBMPlay
        width: 60
        height: 60
        visible: m_pMusicControlDlg.isPlay
        color: "#00000000"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        image:"images/home_control/device/bofang.png"
        imagepressed: "images/home_control/device/zanting.png"
        onClicked: {
            console.log(m_pMusicControlDlg.musicPowerStat())
            if(!m_pMusicControlDlg.musicPowerStat())
                return
            m_pMusicControlDlg.on_pushButtonBMPlay_clicked()
            m_pMusicControlDlg.isPlay = false
            //pushButtonBMPause.visible=true
            //pushButtonBMPlay.visible=false
        }
    }
    Button {//暂停
        id: pushButtonBMPause
        width: 60
        height: 60
        color: "#00000000"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        image:"images/home_control/device/zanting.png"
        imagepressed: "images/home_control/device/bofang.png"
        visible: !m_pMusicControlDlg.isPlay
        onClicked: {
            console.log(m_pMusicControlDlg.musicPowerStat())
            if(!m_pMusicControlDlg.musicPowerStat())
                return
            m_pMusicControlDlg.on_pushButtonBMPause_clicked()
            m_pMusicControlDlg.isPlay = true
            //pushButtonBMPause.visible=false
            //pushButtonBMPlay.visible=true
        }
    }

    Button {
        id: pushButtonBMNextMusic
        width: 39
        height: 22
        color:"#00000000"
        anchors.right: parent.right
        anchors.rightMargin: 100
        anchors.verticalCenter: parent.verticalCenter
        image:"images/home_control/device/xiayishou.png"
        imagepressed: "images/home_control/device/xiayishou-1.png"
        onClicked: m_pMusicControlDlg.on_pushButtonBMNextMusic_clicked()
    }
}
