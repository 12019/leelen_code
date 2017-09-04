import QtQuick 1.0

Item {
    id: cloudsetting
    width: 800
    height: 480
    objectName: "cloudsetting"
    LocalFontLoader{
        id:localfont
    }

    TextEdit {
        id: lin_Cloud_IP
        text: m_pHCSetting.getSvrIPText()
        activeFocusOnPress: false
        readOnly: true
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.verticalCenterOffset: -150
        anchors.verticalCenter: parent.verticalCenter
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 25
        font.family: localfont.name
    }

    Rectangle {
        id: lin_Cloud_IPline
        height: 1
        color: "#000000"
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.top: lin_Cloud_IP.bottom
        anchors.topMargin: 0
        anchors.left: lin_Cloud_IP.right
        anchors.leftMargin: 0
    }
    TextEdit {
        id: lin_Cloud_IPEdit
        height: lin_Cloud_IP.height
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.left: lin_Cloud_IP.right
        anchors.leftMargin: 0
        anchors.bottom: lin_Cloud_IPline.top
        anchors.bottomMargin: 1
        font.pixelSize: 25
        font.family: localfont.name
        text: m_pHCSetting.lin_CloudIP
        anchors.right: parent.right
        anchors.rightMargin: 50
        activeFocusOnPress: false
        readOnly: true
        MouseArea{
            anchors.fill: parent
            onClicked: {
                m_pHCSetting.setCloundIpText(lin_Cloud_IPEdit.text)
            }
        }
    }



    TextEdit {
        id: lin_LocalID
        text: m_pHCSetting.getSvrIDText()
        activeFocusOnPress: false
        readOnly: true
        anchors.verticalCenterOffset: 150
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 50
        font.pixelSize: 25
        font.family: localfont.name
    }
    Rectangle {
        id: lin_LocalIDline
        height: 1
        color: "#000000"
        anchors.top: lin_LocalID.bottom
        anchors.topMargin: 0
        width: lin_Cloud_IPline.width
        x:lin_Cloud_IPline.x
    }
    TextEdit {
        id: localIDText
        width: lin_Cloud_IPline.width
        height: lin_Cloud_IP.height
        x:lin_Cloud_IPline.x
        anchors.bottom: lin_LocalIDline.top
        anchors.bottomMargin: 1
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 25
        font.family: localfont.name
        text: m_pHCSetting.lin_LocalID
        activeFocusOnPress: false
        readOnly: true
        MouseArea{
            anchors.fill: parent
            onClicked: {
                m_pHCSetting.setCloundIDText(localIDText.text)
            }
        }
    }



    TextEdit {
        id: lin_Account
        text: m_pHCSetting.getSvrAccText()
        anchors.verticalCenterOffset: -50
        anchors.verticalCenter: parent.verticalCenter
        activeFocusOnPress: false
        readOnly: true
        anchors.left: parent.left
        anchors.leftMargin: 50
        font.pixelSize: 25
        font.family: localfont.name
    }
    Rectangle {
        id: lin_AccountLine
        height: 1
        width: lin_Cloud_IPline.width
        color: "#000000"
        x:lin_Cloud_IPline.x
        anchors.top: lin_Account.bottom
        anchors.topMargin: 0
    }
    TextEdit {
        id: lin_AccountEdit
        width: lin_Cloud_IPline.width
        height: lin_Cloud_IP.height
        anchors.bottom: lin_AccountLine.top
        anchors.bottomMargin: 0
        x: lin_Cloud_IPline.x
        font.pixelSize: 25
        font.family: localfont.name
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text:m_pHCSetting.lin_CloudAcc
        MouseArea{
            anchors.fill: parent
            onClicked: {
                m_pHCSetting.setCloundAccountText(lin_AccountEdit.text)
            }
        }
    }



    TextEdit {
        id: lin_Pwd
        text: m_pHCSetting.getSvrPwdText()
        activeFocusOnPress: false
        readOnly: true
        anchors.verticalCenterOffset: 50
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 50
        font.pixelSize: 25
        font.family: localfont.name
    }
    Rectangle {
        id: lin_PwdLine
        width: lin_Cloud_IPline.width
        height: 1
        x:lin_Cloud_IPline.x
        color: "#000000"
        anchors.top: lin_Pwd.bottom
        anchors.topMargin: 0
    }
    TextEdit {
        id: lin_PwdEdit
        width: lin_Cloud_IPline.width
        height: lin_Cloud_IP.height
        anchors.bottom: lin_PwdLine.top
        anchors.bottomMargin: 1
        x: lin_Cloud_IPline.x
        font.pixelSize: 25
        font.family: localfont.name
        text:m_pHCSetting.lin_CloudPwd
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        MouseArea{
            anchors.fill: parent
            onClicked: {
                m_pHCSetting.setCloundPWDText(lin_PwdEdit.text)
            }
        }
    }
}
