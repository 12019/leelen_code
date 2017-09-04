import QtQuick 1.0

Item {
    id: serialPortInfo
    width: 800
    height: 480

    Timer {
        id:timer
        interval: 2000
        running: false
        repeat: false
        onTriggered:{
            m_pappWindow.setloading(false)
            serialPortText.text = m_pHCSetting.serialInfo()
            action.start()
        }
    }

    LocalFontLoader{
        id:localfont
    }


    TextEdit {
        id: serialPortText
        //text: "家居版本号:  1.1.4"
        anchors.right: parent.right
        anchors.rightMargin: 100
        anchors.left: parent.left
        anchors.leftMargin: 100
        activeFocusOnPress: false
        readOnly: true
        anchors.verticalCenterOffset: 100
        anchors.verticalCenter: parent.verticalCenter
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 30
        font.family: localfont.name
        //visible: false
        opacity: 0
    }

    Rectangle {
        id: rectangle1
        x: 273
        y: 110
        width: 200
        height: 100
        radius: 28
        border.width: 5
        border.color: mouse.pressed?"#eb0f0f":"#11e079"

        anchors.verticalCenterOffset: -100
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        TextEdit {
            id: textEdit1
            text: m_pHCSetting.getSerialLabelText()//"串口检测"
            readOnly: true
            activeFocusOnPress: false
            clip: true
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 25
            font.family: localfont.name
        }
        MouseArea{
            id:mouse
            anchors.fill: parent
            onClicked: {
                m_pappWindow.setloading(true)
                timer.stop()
                serialPortText.opacity = 0
                m_pHCSetting.serialdetect()
                timer.start()
            }
        }
    }

    PropertyAnimation{
        id:action
        target: serialPortText
        property: "opacity"
        from:0
        to:1
        duration: 1000
    }
}
