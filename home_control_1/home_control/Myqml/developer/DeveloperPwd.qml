import QtQuick 1.0
import "../content"

Item{
    id: item
    width: 800
    height: 480
    opacity: 1
    property real mFontSize: 25


    function adaptInterface(){
        mFontSize = mFontSize*m_pappWindow.screenPerInch()
    }

    LocalFontLoader{
        id:localfont
    }
    Rectangle{
        id:developerpwd_rec
        border.color: "#0f824b"
        border.width: 2
        anchors.left: parent.left
        anchors.leftMargin: 150
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 150
        height: mFontSize+10
        Text {
            id:developerpwd_label
            text: m_pParaStyle.getLabelText("developerpwd_pwd")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: mFontSize
            font.family: localfont.name
            anchors.fill: parent

        }
        TextEdit{
            id: pwd
            text: m_pParaStyle.userCode
            //echoMode: TextInput.PasswordEchoOnEdit
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: mFontSize
            font.family: localfont.name
            onTextChanged: {
                if(text.length == ""){
                    developerpwd_label.text = m_pParaStyle.getLabelText("developerpwd_pwd")
                }
                else{
                    developerpwd_label.text = ""
                }

            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    //m_pHCSetting.setTerAccountText(pwd.text)
                    m_pParaStyle.setDeveloperPwdText(pwd.text)
                }
            }
        }
    }



    Rectangle {
        id: deceloperRec
        //width: pwd.height
        height: developerpwd_rec.height
        anchors.right: parent.right
        anchors.rightMargin: 80
        anchors.top: developerpwd_rec.top
        anchors.topMargin: 0
        anchors.left: developerpwd_rec.right
        anchors.leftMargin: 10
        Text {
            id: developerPwdLabel
            text:m_pParaStyle.getLabelText("developerPwdLabel")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: mFontSize
            font.family: localfont.name
            anchors.fill: parent
        }
        MouseArea{
            anchors.fill: parent
            id:btnmouse
            onClicked: {
                if(pwd.text == "99999999"){
                    //pwd.text = ""
                    var msg = m_pParaStyle.getLabelText("pwdIsValid")
                    console.log(msg)
                    //m_pappWindow.showToast("密码正确")
                    m_pDeveloper.setState("developer/Developer.qml")
                }
                else if(pwd.text != "999999"){
                    developerpwd_label.text = m_pParaStyle.getLabelText("developerpwd_pwd")
                    m_pParaStyle.userCode = ""
                    var msgerror = m_pParaStyle.getLabelText("pwdIsInValid")
                    //m_pappWindow.showToast("密码错误")
                    //m_pDeveloper.setState("developer/Developer.qml")
                }
            }
        }
    }
}
