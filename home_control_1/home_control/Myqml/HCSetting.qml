import QtQuick 1.0
import "developer"
Rectangle {
    id:setting
    width: 800
    height: 480
    color: "#ffffff"
    objectName: "setting"

    property string quxiao: "images/home_control/list/quxiao.png"
    property string quxiao_1: "images/home_control/list/quxiao-1.png"
    property string wancheng : "images/home_control/list/wancheng.png"
    property string wancheng_1 : "images/home_control/list/wancheng-1.png"

    MouseArea{
        anchors.fill: parent
    }


    LocalFontLoader{
        id:localfont
    }

    /***********************************************************************************
    函数名 :setDevListView
    描  述 : 调用设置界面的函数，stat为要载入的qml文件
    输入参数说明 :
    输出参数说明 :
    返 回 值:
    其它说明:
    ***********************************************************************************/
    function setDevListView(_name,_id,_type){
        if(objectItem.item.objectName == "Developer"){
            objectItem.item.setDevView(_name,_id,_type)
        }
    }

    function setListModelText(){
        settingModel.clear()
        settingModel.append({"title":m_pHCSetting.getSvrSetLabelText()})
        settingModel.append({"title":m_pHCSetting.getTerSetLabelText()})
        settingModel.append({"title":m_pHCSetting.getSerialLabelText()})
        settingModel.append({"title":m_pParaStyle.getLabelText("developer")})
        settingModel.append({"title":m_pHCSetting.getHomeLabelText()})
    }

    ListModel {
        id: settingModel
        ListElement {
            title: ""
        }
    }

    Component {
        id: settingDelegate
        Item{
           width: settingList.width
           height: 147
           Image {
               source: settingList.currentIndex==index?
                           "images/home_control/list/settingList-1.png":
                           "images/home_control/list/settingList.png"
               anchors.fill: parent
           }
           TextEdit {
               id: name
               text: title
               font.family: localfont.name
               font.pixelSize: 25
               anchors.horizontalCenter: parent.horizontalCenter
               anchors.verticalCenter: parent.verticalCenter
               readOnly: true
               cursorVisible: false
           }
           Rectangle{
               width:settingList.width - 25
               height: 1
               color: "#000000"
               anchors.bottom: parent.bottom
           }
           MouseArea{
               anchors.fill: parent
               onClicked: {
                   m_pappWindow.setloading(false)
                   settingList.currentIndex = index
                   if(index == 0){
                       setState("CloudSetting.qml")
                   }
                   else if(index == 1){
                       setState("TerminalRemoteSet.qml")
                   }
                   else if(index == 2){
                       setState("HCserialPortInfo.qml")
                   }
                   else if(index == 3){
                       setState("developer/DeveloperPwd.qml")
                   }
                   else if(index == 4){
                       setState("HCVersion.qml")
                   }
               }
               onPressed: {
                   settingList.flag = true
               }
               onCanceled: {
                   if(settingList.flag){
                       settingList.currentIndex = index
                       if(index == 0){
                           setState("CloudSetting.qml")
                       }
                       else if(index == 1){
                           setState("TerminalRemoteSet.qml")
                       }
                       else if(index == 2){
                           setState("HCserialPortInfo.qml")
                       }
                       else if(index == 3){
                           setState("developer/DeveloperPwd.qml")
                       }
                       else if(index == 4){
                           setState("HCVersion.qml")
                       }
                   }
               }
           }
        }
    }

    ListView {
        ScrollBar
        {
            scrollArea: settingList
            height: settingList.height
            width: 10
            x:settingList.width-10
        }
        anchors.top: cancel.bottom
        anchors.topMargin: 0
        property bool flag: true
        id:settingList
        width: 255
        height: setting.height
        model: settingModel
        delegate: settingDelegate
        currentIndex: -1
        onMovementStarted: {
            settingList.flag = false
        }
    }

    Rectangle {
        id: cancel
        height: 39
        color: "#000000"
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        Image {
            id: cancelImg
            source: quxiao
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
        }
        TextEdit{
            text: m_pHCSetting.getCloseLabelText()//"退出"
            anchors.left: cancelImg.right
            anchors.leftMargin: -20
            anchors.verticalCenter: parent.verticalCenter
            font.family: localfont.name
            font.pixelSize: 18
            color: "#ffffff"
        }
        MouseArea{
            width: parent.width/2/3
            height: parent.height

            onPressed: {
                cancelImg.source = quxiao_1
            }
            onReleased: {
                cancelImg.source = quxiao
                m_pappWindow.setMainState("")
            }
        }



        Image {
            id: sure
            anchors.right: parent.right
            anchors.rightMargin: -40
            source: wancheng
            anchors.verticalCenter: parent.verticalCenter
        }
        TextEdit{
            text: m_pHCSetting.getFinishLabelText()//"确定"
            anchors.right: parent.right
            anchors.rightMargin: 50
            anchors.verticalCenter: parent.verticalCenter
            font.family: localfont.name
            font.pixelSize: 18
            color: "#ffffff"
        }
        MouseArea{
            width: parent.width/2/3
            height: parent.height
            anchors.right: parent.right
            anchors.rightMargin: 0
            onPressed: {
                sure.source = wancheng_1
            }
            onReleased: {
                sure.source = wancheng
                m_pappWindow.setMainState("")
            }
        }
    }

    function setState(stat){
        objectItem.source = stat
        objectItem.item.width = setting.width - settingList.width
        objectItem.item.height = setting.height - cancel.height
    }

    Loader{
        id:objectItem
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: cancel.bottom
        anchors.topMargin: 0
        anchors.left: settingList.right
        anchors.leftMargin: 0
        onStatusChanged: {
            if(objectItem.status == Loader.Ready){
                console.log("loaded")
            }
            else if(objectItem.status == Loader.Loading){
                console.log("loading")
            }
            else if(objectItem.status == Loader.Null){
                console.log("null")
            }
        }
    }
}
