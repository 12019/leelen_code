import QtQuick 1.0

Item {
    id: cloudsetting
    width: 800
    height: 480


    LocalFontLoader{
        id:localfont
    }

    TextEdit {
        id: account
        text: m_pHCSetting.getTerAccountText()
        activeFocusOnPress: false
        readOnly: true
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.verticalCenterOffset: -50
        anchors.verticalCenter: parent.verticalCenter
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 25
        font.family: localfont.name
    }

    TextEdit {
        id: pwd
        text: m_pHCSetting.getTerPwdText()
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
        id: accountline
        height: 1
        color: "#000000"
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.top: account.bottom
        anchors.topMargin: 0
        anchors.left: account.right
        anchors.leftMargin: 0
    }

    Rectangle {
        id: pwdline
        height: 1
        color: "#000000"
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.top: pwd.bottom
        anchors.topMargin: 0
        anchors.left: account.right
        anchors.leftMargin: 0
    }

    TextEdit {
        id: ipText
        height: account.height
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.left: account.right
        anchors.leftMargin: 0
        anchors.bottom: accountline.top
        anchors.bottomMargin: 2
        font.pixelSize: 25
        font.family: localfont.name
        text: m_pHCSetting.terAccount
        anchors.right: parent.right
        anchors.rightMargin: 50
        activeFocusOnPress: false
        readOnly: true
        MouseArea{
            anchors.bottomMargin: 1
            anchors.fill: parent
            onClicked: {
                m_pHCSetting.setTerAccountText(ipText.text)
            }
        }
    }

    TextEdit {
        id: pwdText
        height: account.height
        anchors.left: account.right
        anchors.leftMargin: 0
        anchors.bottom: pwdline.top
        anchors.bottomMargin: 1
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 25
        font.family: localfont.name
        text: m_pHCSetting.terPwd
        anchors.right: parent.right
        anchors.rightMargin: 50
        MouseArea{
            anchors.fill: parent
            onClicked: {
                m_pHCSetting.setTerPwdText(pwdText.text)
            }
        }
    }
}
