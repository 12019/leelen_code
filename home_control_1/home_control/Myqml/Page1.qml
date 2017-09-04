import QtQuick 1.0

Item {
    width: 460
    height: 401  //240

    property string setting: "images/home_control/scene/setting.png"
    property string setting_1: "images/home_control/scene/setting-1.png"

    Button{
            id:pushButtonSetting
            width: 74
            height: 75
            onClicked: {
                m_pappWindow.setMainState("setting")
            }
            image:setting//"images/home_control/scene/qichuang.png"
            imagepressed: setting_1//"images/home_control/scene/qichuang-1.png"
            color: "#00000000"
            anchors.top: parent.top
            anchors.topMargin: 50
            anchors.left: parent.left
            anchors.leftMargin: 63
            onPressed: {
                //view.flag = 1
            }
            onCancel: {
                //if(view.flag == 1){
                    m_pappWindow.setMainState("setting")
                //}
                //view.flag = 1
            }
    }
    TextEdit{
        id:settingLabel
        width: 71
        height: 24
        text: m_pHCSetting.getSettingNameText()//"设置"//m_pHCSceneDialog.s1text//"起床模式"
        anchors.left: parent.left
        anchors.leftMargin: 62
        anchors.top: pushButtonSetting.bottom
        anchors.topMargin: 0
        font.family: localfont.name
        font.pixelSize: 18
        activeFocusOnPress: false
        readOnly: true
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        cursorVisible: false
    }
}
