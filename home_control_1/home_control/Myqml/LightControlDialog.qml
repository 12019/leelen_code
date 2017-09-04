// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.0

Rectangle {
    id:light
    width: 372
    height: 206
    color: "#00000000"
    opacity: 1
    radius: 10

    property int repeatDelay: 1000
    property int repeatDuration: 50
    property bool addPress: false
    property bool minuPress:false
    property bool pressed: false

    function release() {
            autoRepeatClicks.stop()
            light.pressed = false
    }

    LocalFontLoader{
        id:localfont
    }

    function setBrightenValue(){
        if(light.addPress){
            if(Number(m_pLightDialog.value) <= 100){
                if(Number(m_pLightDialog.value) + 3 >= 100){
                    m_pLightDialog.on_pushButtonBrightness_clicked(100)
                }
                else{
                    m_pLightDialog.on_pushButtonBrightness_clicked(Number(m_pLightDialog.value)+3)
                }
            }
        }
        else if(light.minuPress){
            if(Number(m_pLightDialog.value) >= 0){
                if(Number(m_pLightDialog.value) - 3 < 0){
                    m_pLightDialog.on_pushButtonBrightness_clicked(0)
                }
                else{
                    m_pLightDialog.on_pushButtonBrightness_clicked(Number(m_pLightDialog.value)-3)
                }
            }
        }
    }
    MouseArea{
        anchors.fill: parent
    }
    BorderImage {
        id: lightbg
        source: "images/home_control/device/chuanglian bg.png"
        anchors.fill: parent
    }

    /***************调光值设置****************/
    Item{
        id:pushButtonBrightness
        y: 76
        anchors.left: parent.left
        anchors.leftMargin: 15
        anchors.verticalCenter: parent.verticalCenter
        width: 160
        height: 55
        Image {
            source: brightnessMinus.pressed?
                        "images/home_control/device/1-1.png":
                       "images/home_control/device/1.png"
            width: 50
            height: 55
            x: 0
            y: 0
            MouseArea{
                id:brightnessMinus
                anchors.fill: parent
                onClicked: {
                    if(m_pLightDialog.stat =="开" | m_pLightDialog.stat == "on"){
                        if(Number(m_pLightDialog.value)-1 >= 0)
                            m_pLightDialog.on_pushButtonBrightness_clicked(Number(m_pLightDialog.value)-1)
                    }
                }
                onPressed: {
                    light.minuPress = true
                    autoRepeatClicks.start()
                }
                onReleased: {
                    light.release()
                    light.minuPress = false
                }
            }
        }
        Rectangle{
            x: 50
            y: 0
            width: 100
            height: 48
        }
        TextEdit{
            x: 50
            y: 0
            width: 60
            height: 48
            color: "#000000"
            text: m_pLightDialog.value+"%"
            font.family: localfont.name
            font.pixelSize: 18
            readOnly: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        Image {
            source: brightnessAdd.pressed?
                        "images/home_control/device/2-1.png":
                       "images/home_control/device/2.png"
            width: 50
            height: 55
            anchors.right: parent.right
            anchors.rightMargin: 0
            MouseArea{
                id:brightnessAdd
                anchors.fill: parent
                onClicked: {
                    if(m_pLightDialog.stat =="开" | m_pLightDialog.stat == "on"){
                        if(Number(m_pLightDialog.value)+1 <= 100)
                            m_pLightDialog.on_pushButtonBrightness_clicked(Number(m_pLightDialog.value)+1)
                    }
                }
                onPressed: {
                    light.addPress = true
                    autoRepeatClicks.start()
                }
                onReleased: {
                    light.release()
                    light.addPress = false
                }
            }
        }
    }

    Item{
        id:on_pushButtonPower
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.verticalCenter: parent.verticalCenter
        width: 160
        height: 55
        Image {
            source: buttonPowerPre.pressed?
                        "images/home_control/device/1-1.png":
                       "images/home_control/device/1.png"
            width: 50
            height: 55
            x: 0
            y: 0
            MouseArea{
                id:buttonPowerPre
                anchors.fill: parent
                onClicked: {
                    m_pLightDialog.on_pushButtonPower_click(m_pLightDialog.stat)
                }
            }
        }
        Rectangle{
            x: 50
            y: 0
            width: 100
            height: 48
        }
        TextEdit{
            x: 50
            y: 0
            width: 60
            height: 48
            color: "#000000"
            text: m_pLightDialog.stat
            font.family: localfont.name
            font.pixelSize: 18
            readOnly: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        Image {
            source: buttonPowerNext.pressed?
                        "images/home_control/device/2-1.png":
                       "images/home_control/device/2.png"
            width: 50
            height: 55
            anchors.right: parent.right
            anchors.rightMargin: 0
            MouseArea{
                id:buttonPowerNext
                anchors.fill: parent
                onClicked: {
                        m_pLightDialog.on_pushButtonPower_click(m_pLightDialog.stat)
                    }
                }
            }
        }

    TextEdit {
        font.family: localfont.name
        font.pixelSize:18
        text: m_pLightDialog.label
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 150
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

    SequentialAnimation on pressed {
            id: autoRepeatClicks
            running: false
            PropertyAction { target: light; property: "pressed"; value: true }
            //ScriptAction { script: light.setBrightenValue() }
            PauseAnimation { duration: repeatDelay }
            SequentialAnimation {
                loops: Animation.Infinite
                ScriptAction { script: light.setBrightenValue()}
                PauseAnimation { duration: repeatDuration }
            }
        }
}
