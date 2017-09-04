import QtQuick 1.0


Item {
    width: 460
    height: 401  //240
    //color: "#00000000"
    //radius: 10


    property string sceneBG : "images/home_control/scene/bg.png"
    property string qichuang : "images/home_control/scene/qichuang.png"
    property string qichuang_1 : "images/home_control/scene/qichuang-1.png"
    property string yongcan : "images/home_control/scene/yongcan.png"
    property string yongcan_1 : "images/home_control/scene/yongcan-1.png"
    property string yingyin : "images/home_control/scene/yingyin.png"
    property string yingyin_1 : "images/home_control/scene/yingyin-1.png"
    property string shuimian : "images/home_control/scene/shuimian.png"
    property string shuimian_1 : "images/home_control/scene/shuimian-1.png"
    property string scenezidingyiA : "images/home_control/scene/zidingyiA.png"
    property string scenezidingyiA_1 : "images/home_control/scene/zidingyiA-1.png"
    property string scenezidingyiB : "images/home_control/scene/zidingyiB.png"
    property string scenezidingyiB_1 : "images/home_control/scene/zidingyiB-1.png"
    property string scenequankai : "images/home_control/scene/quankai.png"
    property string scenequankai_1 : "images/home_control/scene/quankai-1.png"
    property string scenequanguan : "images/home_control/scene/quanguan.png"
    property string scenequanguan_1 : "images/home_control/scene/quanguan-1.png"
    property string scenetianjia: "images/home_control/scene/tianjia.png"
    property string scenetianjia_1 : "images/home_control/scene/tianjia-1.png"


    /*MouseArea{
        anchors.fill: parent

    }*/

    /*LocalFontLoader{
        id:localfont
    }*/

    /*Image{
        id: bor
        source: sceneBG//"images/home_control/scene/bg.png"
        anchors.fill: parent
    }*/

    Button{
            id:pushButtonScene1
            width: 74
            height: 75
            //name:"起床模式"
            onClicked:m_pHCSceneDialog.pushButtonScenePressed(1)
            image:qichuang//"images/home_control/scene/qichuang.png"
            imagepressed: qichuang_1//"images/home_control/scene/qichuang-1.png"
            color: "#00000000"
            anchors.top: parent.top
            anchors.topMargin: 50
            anchors.left: parent.left
            anchors.leftMargin: 63
            onPressed: {
                //view.flag = 1
                currentPressX = mousex
            }
            onPositionchanged: {
                setState(mousex)
            }

            /*onCancel: {
                if(view.flag == 1){
                    m_pHCSceneDialog.pushButtonScenePressed(1)
                }
                view.flag = 1
            }*/
        }

        Button {
            id: pushButtonScene2
            //x: 193
            width: 74
            height: 75
            //name:"就餐模式"
            image:yongcan//"images/home_control/scene/yongcan.png"
            imagepressed:yongcan_1//"images/home_control/scene/yongcan-1.png"
            onClicked: m_pHCSceneDialog.pushButtonScenePressed(2)
            color: "#00000000"
            anchors.top: parent.top
            anchors.topMargin: 50
            anchors.horizontalCenter: parent.horizontalCenter
            onPressed: {
                //view.flag = 1
                currentPressX = mousex
            }
            onPositionchanged: {
                setState(mousex)
            }
        }

        Button {
            id: pushButtonScene3
            //x: 319
            width: 74
            height: 75
            //name:"影音模式"
            image:yingyin//"images/home_control/scene/yingyin.png"
            imagepressed:yingyin_1//"images/home_control/scene/yingyin-1.png"
            onClicked: m_pHCSceneDialog.pushButtonScenePressed(3)
            color: "#00000000"
            anchors.top: parent.top
            anchors.topMargin: 50
            anchors.right: parent.right
            anchors.rightMargin: 59
            onPressed: {
                //view.flag = 1
                currentPressX = mousex
            }
            onPositionchanged: {
                setState(mousex)
            }
        }

        Button {
            id: pushButtonScene4
            width: 74
            height: 75
            //name:"睡眠"
            image:shuimian//"images/home_control/scene/shuimian.png"
            imagepressed:shuimian_1//"images/home_control/scene/shuimian-1.png"
            onClicked: m_pHCSceneDialog.pushButtonScenePressed(4)
            color: "#00000000"
            anchors.left: parent.left
            anchors.leftMargin: 63
            anchors.verticalCenter: parent.verticalCenter
            onPressed: {
                //view.flag = 1
                currentPressX = mousex
            }
            onPositionchanged: {
                setState(mousex)
            }
        }

        Button {
            id: pushButtonScene5
            width: 74
            height: 75
            //name:"自定义A"
            image:scenezidingyiA//"images/home_control/scene/zidingyiA.png"
            imagepressed:scenezidingyiA_1//"images/home_control/scene/zidingyiA-1.png"
            onClicked: m_pHCSceneDialog.pushButtonScenePressed(5)
            color: "#00000000"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            onPressed: {
                //view.flag = 1
                currentPressX = mousex
            }
            onPositionchanged: {
                setState(mousex)
            }
        }

        Button {
            id: pushButtonScene6
            width: 74
            height: 75
            //name:"影音"
            image:scenezidingyiB//"images/home_control/scene/zidingyiB.png"
            imagepressed:scenezidingyiB_1//"images/home_control/scene/zidingyiB-1.png"
            onClicked: m_pHCSceneDialog.pushButtonScenePressed(6)
            color: "#00000000"
            anchors.right: parent.right
            anchors.rightMargin: 59
            anchors.verticalCenter: parent.verticalCenter
            onPressed: {
                //view.flag = 1
                currentPressX = mousex
            }
            onPositionchanged: {
                setState(mousex)
            }
        }

        Button {
            id: pushButtonScene7
            width: 74
            height: 75
            //name:"全开"
            image:scenequankai//"images/home_control/scene/quankai.png"
            imagepressed:scenequankai_1//"images/home_control/scene/quankai-1.png"
            onClicked: m_pHCSceneDialog.pushButtonScene7Pressed()
            color: "#00000000"
            anchors.left: parent.left
            anchors.leftMargin: 63
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 50
            onPressed: {
                //view.flag = 1
                currentPressX = mousex
            }
            onPositionchanged: {
                setState(mousex)
            }
        }

        Button {
            id: pushButtonScene8
            width: 74
            height: 75
            //name:"全关"
            image:scenequanguan//"images/home_control/scene/quanguan.png"
            imagepressed:scenequanguan_1//"images/home_control/scene/quanguan-1.png"
            onClicked: m_pHCSceneDialog.pushButtonScene8Pressed()
            color: "#00000000"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 50
            anchors.horizontalCenter: parent.horizontalCenter
            onPressed: {
                //view.flag = 1
                currentPressX = mousex
            }
            onPositionchanged: {
                setState(mousex)
            }
        }

        Button {
            id: edit
            width: 74
            height: 75
            //name:"编辑"
            image:scenetianjia//"images/home_control/scene/tianjia.png"
            imagepressed:scenetianjia_1//"images/home_control/scene/tianjia-1.png"
            onClicked: {
                //mainRec.hcsceneeditEvent("sceneModel","","","","")
                //mainRec.hcsceneeditEvent("clearModel","","","","")
                m_pHCSceneDialog.pushButtonScene9Pressed();
            }
            color: "#00000000"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 50
            anchors.right: parent.right
            anchors.rightMargin: 59
            onPressed: {
                //view.flag = 1
                currentPressX = mousex
            }
            onPositionchanged: {
                setState(mousex)
            }
        }

        TextEdit{
            id:wakeup
            //x: 71
            width: 71
            height: 24
            text: m_pHCSceneDialog.s1text//"起床模式"
            anchors.left: parent.left
            anchors.leftMargin: 62
            anchors.top: pushButtonScene1.bottom
            anchors.topMargin: 0
            font.family: localfont.name
            font.pixelSize: 18
            activeFocusOnPress: false
            readOnly: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            cursorVisible: false
        }
        TextEdit{
            id:repast
            //x: 191
            width: 71
            height: 24
            text: m_pHCSceneDialog.s2text//"就餐模式"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: pushButtonScene2.bottom
            anchors.topMargin: 0
            font.family: localfont.name
            font.pixelSize: 18
            activeFocusOnPress: false
            readOnly: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            cursorVisible: false
        }
        TextEdit{
            id:movie
            //x: 319
            width: 71
            height: 24
            text: m_pHCSceneDialog.s3text//"影音模式"
            anchors.right: parent.right
            anchors.rightMargin: 60
            anchors.top: pushButtonScene3.bottom
            anchors.topMargin: 0
            font.family: localfont.name
            font.pixelSize: 18
            activeFocusOnPress: false
            readOnly: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            cursorVisible: false
        }
        TextEdit{
            id:sleep
            width: 71
            height: 24
            text: m_pHCSceneDialog.s4text //"睡眠模式"
            anchors.left: parent.left
            anchors.leftMargin: 62
            anchors.top: pushButtonScene4.bottom
            anchors.topMargin: 0
            font.family: localfont.name
            font.pixelSize: 18
            activeFocusOnPress: false
            readOnly: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            cursorVisible: false
        }
        TextEdit{
            id:customA
            //x: 191
            width: 71
            height: 24
            text: m_pHCSceneDialog.s5text//"自定义A"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: pushButtonScene5.bottom
            anchors.topMargin: 0
            font.family: localfont.name
            font.pixelSize: 18
            activeFocusOnPress: false
            readOnly: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            cursorVisible: false
        }
        TextEdit{
            id:customB
            //x: 319
            width: 71
            height: 24
            text: m_pHCSceneDialog.s6text//"自定义B"
            anchors.right: parent.right
            anchors.rightMargin: 60
            anchors.top: pushButtonScene6.bottom
            anchors.topMargin: 0
            font.family: localfont.name
            font.pixelSize: 18
            activeFocusOnPress: false
            readOnly: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            cursorVisible: false
        }
        TextEdit{
            id:fullopen
            width: 71
            height: 24
            text: m_pHCSceneDialog.s7text//"居家"
            anchors.left: parent.left
            anchors.leftMargin: 62
            anchors.top: pushButtonScene7.bottom
            anchors.topMargin: 0
            font.family: localfont.name
            font.pixelSize: 18
            activeFocusOnPress: false
            readOnly: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            cursorVisible: false
        }
        TextEdit{
            id:fullclose
            //x: 191
            width: 71
            height: 24
            text: m_pHCSceneDialog.s8text//"外出"
            anchors.top: pushButtonScene7.bottom
            anchors.topMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: localfont.name
            font.pixelSize: 18
            activeFocusOnPress: false
            readOnly: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            cursorVisible: false
        }
        TextEdit{
            id:compile
            //x: 319
            width: 71
            height: 24
            text: m_pHCSceneDialog.s9text//"编辑"
            anchors.top: edit.bottom
            anchors.topMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 60
            font.family: localfont.name
            font.pixelSize: 18
            activeFocusOnPress: false
            readOnly: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            cursorVisible: false
        }
}

