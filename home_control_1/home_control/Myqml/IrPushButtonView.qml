import QtQuick 1.0
Rectangle{
    width: 800
    height: 480
    opacity: 1
    color: "#00000000"
    id:irlearnpwd

    property string pwd:""
    property string star: ""
    property string borImg:"images/home_control/irpasswd/bg.png"
    property string num1_1: "images/home_control/irpasswd/1-1.png"// This is available in all editors.
    property string num1_2: "images/home_control/irpasswd/1-2.png"
    property string num2_1: "images/home_control/irpasswd/2-1.png"
    property string num2_2: "images/home_control/irpasswd/2-2.png"
    property string num3_1: "images/home_control/irpasswd/3-1.png"
    property string num3_2: "images/home_control/irpasswd/3-2.png"
    property string num4_1: "images/home_control/irpasswd/4-1.png"
    property string num4_2: "images/home_control/irpasswd/4-2.png"
    property string num5_1: "images/home_control/irpasswd/5-1.png"
    property string num5_2: "images/home_control/irpasswd/5-2.png"
    property string num6_1: "images/home_control/irpasswd/6-1.png"
    property string num6_2: "images/home_control/irpasswd/6-2.png"
    property string num7_1: "images/home_control/irpasswd/7-1.png"
    property string num7_2: "images/home_control/irpasswd/7-2.png"
    property string num8_1: "images/home_control/irpasswd/8-1.png"
    property string num8_2: "images/home_control/irpasswd/8-2.png"
    property string num9_1: "images/home_control/irpasswd/9-1.png"
    property string num9_2: "images/home_control/irpasswd/9-2.png"
    property string num10_1: "images/home_control/irpasswd/10-1.png"
    property string num10_2: "images/home_control/irpasswd/10-2.png"
    property string num11_1: "images/home_control/irpasswd/11-1.png"
    property string num11_2: "images/home_control/irpasswd/11-2.png"
    property string num0_1: "images/home_control/irpasswd/0-1.png"
    property string num0_2: "images/home_control/irpasswd/0-2.png"

    LocalFontLoader{
        id:localfont
    }

    MouseArea{
        anchors.fill: parent
        onReleased: {
            irlearnpwd.star = ""
            irlearnpwd.pwd = ""
            //irpwd.text = "密码编辑框"
            m_pIrpasswdDlg.close()
        }
    }

    Timer{
        id:timer
        interval: 500
        running: false
        repeat: false
        onTriggered: {
            m_pIrpasswdDlg.setPwdName("pwd")
        }
    }

    BorderImage {
        anchors.fill: parent
        source: borImg//"images/home_control/irpasswd/bg.png"
    }
    Button {
        id:one
        //name:"1"
        image: num1_1//"images/home_control/irpasswd/1-1.png"
        imagepressed: num1_2//"images/home_control/irpasswd/1-2.png"
        color: "#00000000"
        anchors.left: parent.left
        anchors.leftMargin: 230
        anchors.top: parent.top
        anchors.topMargin: 110
        width: 77
        height: 76
        fontsize: 50
        onClicked: {
            irlearnpwd.star+="●"
            m_pIrpasswdDlg.passwd = irlearnpwd.star
            irlearnpwd.pwd+="1"
        }
        opacity: 1
    }

    Button {
        id: two
        color: "#00000000"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 110
        //name:"2"
        image: num2_1//"images/home_control/irpasswd/2-1.png"
        imagepressed: num2_2//"images/home_control/irpasswd/2-2.png"
        x: 350
        width: 77
        height: 76
        fontsize: 50
        onClicked: {
            irlearnpwd.star+="●"
            m_pIrpasswdDlg.passwd = irlearnpwd.star
            irlearnpwd.pwd+="2"
        }
    }

    Button {
        id: three
        //name:"3"
        color: "#00000000"
        anchors.right: parent.right
        anchors.rightMargin: 230
        anchors.top: parent.top
        anchors.topMargin: 110
        image: num3_1//"images/home_control/irpasswd/3-1.png"
        imagepressed: num3_2//"images/home_control/irpasswd/3-2.png"
        x: 470
        width: 77
        height: 76
        fontsize: 50
        onClicked: {
            irlearnpwd.star+="●"
            m_pIrpasswdDlg.passwd = irlearnpwd.star
            irlearnpwd.pwd+="3"
        }
    }

    Button {
        id: four
        color: "#00000000"
        anchors.left: parent.left
        anchors.leftMargin: 230
        anchors.top: one.bottom
        anchors.topMargin: 15
        image: num4_1//"images/home_control/irpasswd/4-1.png"
        imagepressed: num4_2//"images/home_control/irpasswd/4-2.png"
        width: 77
        height: 76
        //name:"4"
        fontsize: 50
        onClicked: {
            irlearnpwd.star+="●"
            m_pIrpasswdDlg.passwd = irlearnpwd.star
            irlearnpwd.pwd+="4"
        }
    }

    Button {
        id: five
        x: 350
        color: "#00000000"
        anchors.top: two.bottom
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
        image: num5_1//"images/home_control/irpasswd/5-1.png"
        imagepressed: num5_2//"images/home_control/irpasswd/5-2.png"
        width: 77
        height: 76
        //name:"5"
        fontsize: 50
        onClicked: {
            irlearnpwd.star+="●"
            m_pIrpasswdDlg.passwd = irlearnpwd.star
            irlearnpwd.pwd+="5"
        }
    }

    Button {
        id: six
        x: 470
        color: "#00000000"
        anchors.top: three.bottom
        anchors.topMargin: 15
        anchors.right: parent.right
        anchors.rightMargin: 230
        width: 77
        height: 76
        //name:"6"
        image: num6_1//"images/home_control/irpasswd/6-1.png"
        imagepressed: num6_2//"images/home_control/irpasswd/6-2.png"
        fontsize: 50
        onClicked: {
            irlearnpwd.star+="●"
            m_pIrpasswdDlg.passwd = irlearnpwd.star
            irlearnpwd.pwd+="6"
        }
    }

    Button {
        id: seven
        width: 77
        height: 76
        color: "#00000000"
        anchors.top: four.bottom
        anchors.topMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 230
        image: num7_1//"images/home_control/irpasswd/7-1.png"
        imagepressed: num7_2//"images/home_control/irpasswd/7-2.png"
        //name:"7"
        fontsize: 50
        onClicked: {
            irlearnpwd.star+="●"
            m_pIrpasswdDlg.passwd = irlearnpwd.star
            irlearnpwd.pwd+="7"
        }
    }

    Button {
        id: eight
        x: 350
        color: "#00000000"
        anchors.top: five.bottom
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
        image: num8_1//"images/home_control/irpasswd/8-1.png"
        imagepressed: num8_2//"images/home_control/irpasswd/8-2.png"
        width: 77
        height: 76
        fontsize: 50
        //name:"8"
        onClicked: {
            irlearnpwd.star+="●"
            m_pIrpasswdDlg.passwd = irlearnpwd.star
            irlearnpwd.pwd+="8"
        }
    }

    Button {
        id: nine
        x: 470
        width: 77
        height: 76
        color: "#00000000"
        anchors.top: six.bottom
        anchors.topMargin: 15
        anchors.right: parent.right
        anchors.rightMargin: 230
        //name:"9"
        image: num9_1//"images/home_control/irpasswd/9-1.png"
        imagepressed: num9_2//"images/home_control/irpasswd/9-2.png"
        fontsize: 50
        onClicked: {
            irlearnpwd.star+="●"
            m_pIrpasswdDlg.passwd = irlearnpwd.star
            irlearnpwd.pwd+="9"
        }
    }

    Button {
        id: del
        width: 77
        height: 76
        color: "#00000000"
        anchors.top: seven.bottom
        anchors.topMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 230
        image: num10_1//"images/home_control/irpasswd/10-1.png"
        imagepressed: num10_2//"images/home_control/irpasswd/10-2.png"
        //name:"*"
        fontsize: 50
        onClicked: {
            irlearnpwd.star = irlearnpwd.star.substring(0,irlearnpwd.star.length-1)
            m_pIrpasswdDlg.passwd = irlearnpwd.star
            irlearnpwd.pwd = irlearnpwd.pwd.substring(0,irlearnpwd.pwd.length-1)
            if(irlearnpwd.star == "")
                m_pIrpasswdDlg.setPwdName("pwd")
        }
    }

    Button {
        id: zero
        x: 350
        color: "#00000000"
        anchors.top: eight.bottom
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
        image: num0_1//"images/home_control/irpasswd/0-1.png"
        imagepressed: num0_2//"images/home_control/irpasswd/0-2.png"
        width: 77
        height: 76
        //name:"0"
        fontsize: 50
        onClicked: {
            irlearnpwd.star+="●"
            m_pIrpasswdDlg.passwd = irlearnpwd.star
            irlearnpwd.pwd+="0"
        }
    }

    Button {
        id: sure
        x: 470
        image: num11_1//"images/home_control/irpasswd/11-1.png"
        imagepressed: num11_2//"images/home_control/irpasswd/11-2.png"
        width: 77
        height: 76
        //name:"#"
        color: "#00000000"
        anchors.top: nine.bottom
        anchors.topMargin: 15
        anchors.right: parent.right
        anchors.rightMargin: 230
        fontsize: 50
        onClicked: {
            if(irlearnpwd.pwd == "999999"){
                irlearnpwd.star = ""
                m_pIrpasswdDlg.passwd = ""
                irlearnpwd.pwd = ""
                m_pappWindow.setMainState("irlearndlg")
            }
            else{
                //irpwd.text = "密码错误"
                m_pIrpasswdDlg.setPwdName("error")
                timer.start()
                irlearnpwd.star = ""
                irlearnpwd.pwd = ""
            }
        }
    }

    Rectangle{
        id:pawd
        x:230
        width: 340
        height: 62
        radius: 14
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        opacity: 1

        TextEdit{
            id: irpwd
            anchors.fill:parent
            text: m_pIrpasswdDlg.passwd//"密码编辑框"
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
            font.pixelSize: 30
            readOnly: true
        }
    }
}
