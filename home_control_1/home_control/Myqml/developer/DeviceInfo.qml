import QtQuick 1.0
import "../content"
Item {
    id: devInfo
    width: developer.width
    height: developer.height
    property real mFontSize: 20
    property int mLabelW: 80
    property int mLabelH: 30
    property string attr: "light,var,curtain,tv,music,ac,custom"
    property string currenImage : "../images/home_control/list/yixuexi.png"
    property int typeIndex: -1
    property int textType: 0
    property int comboxIndex: 0
    //property string _attr: "不可调光灯,调光灯,窗帘,电视(红外),背景音乐(红外),空调(红外),自定义"

    MouseArea{
        anchors.fill: parent
        onClicked: developer.state = ""
    }

    /*FontLoader {
        id:localfont
        source: "../mnt/disk/lib/fonts/fangzhenglanhei.ttf"
    }*/
    LocalFontLoader{
        id:localfont
    }

    function adaptInterface(){
        mFontSize = mFontSize*m_pappWindow.screenPerInch()
        mLabelH = mLabelH*m_pappWindow.screenPerInch()
        mLabelW = mLabelW*m_pappWindow.screenPerInch()
        devInfoRec.width = devInfoRec.width*m_pappWindow.screenPerInch()
        devInfoRec.height = devInfoRec.height*m_pappWindow.screenPerInch()
    }

    function setDevInfo(_type,_attr,_name,_addr){
        var list = attr.split(",")
        var _TTT = m_pParaStyle.getLabelText("devattrlist")
        var _list = _TTT.split(",")
        for(var i=0;i<7;i++){
            if(String(list.slice(i,i+1)) === _type){
                typeIndex = i
                devTypecomboBox.text = String(_list.slice(i,i+1))
                break
            }
        }
        //setDevAttrList(devTypecomboBox.currentIndex,_attr)

        //devTypecomboBox.text = _type
        devAttrcomboBox.text = _attr
        m_pParaStyle.devName = _name
        m_pParaStyle.devAddr = _addr
        //devNameText.text = _name
        //devAddrText.text = _addr
    }

    function setDevAttrList(index,_attr){
        comboxModel.clear()
        combox.height = 0
        var attr
        var __attr
        switch(index){
        case 0:
        case 2:
            __attr = 1
            for(attr = 1;attr <= 8;attr++){
                comboxModel.append({"_label":attr,"_image":""})
                if(combox.height < devInfo.height){
                    combox.height += mLabelH
                    console.log(combox.height)
                }
            }
            break
        case 1:
            __attr = 9
            for(attr = 9;attr <= 12;attr++){
                comboxModel.append({"_label":attr,"_image":""})
                if(combox.height < devInfo.height){
                    combox.height += mLabelH
                    console.log(combox.height)
                }
            }
            //devAttrcomboBox.currentIndex = _attr - attr
            break
        case 3:
        case 4:
        case 5:
            __attr = 13
            attr = 13
            comboxModel.append({"_label":attr,"_image":""})
            if(combox.height < devInfo.height){
                combox.height += mLabelH
                console.log(combox.height)
            }
            break
        case 6:
            __attr = 14
            for(attr = 14;attr <= 236;attr++){
                comboxModel.append({"_label":attr,"_image":""})
                if(combox.height < devInfo.height){
                    combox.height += mLabelH
                    console.log(combox.height)
                }
            }
            break
        default:
            break
        }
        combox.currentIndex = _attr - __attr
        comboxModel.setProperty(combox.currentIndex,"_image",currenImage)
        devInfo.state = "ShowComBox"
        //devAttrcomboBox.currentIndex = _attr - __attr
    }


    Rectangle {
        id: devInfoRec
        width: 400
        height: 250
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        color: "#af000000"
        radius: 1
        //anchors.top: parent.top
        //anchors.topMargin: -125
        MouseArea{
            id: mouseArea1
            width: 400
            height: 250
            anchors.fill: parent
        }
        TextEdit {
            id: devType
            width: mLabelW
            height: mLabelH
            color: "#ffffff"
            text: m_pParaStyle.getLabelText("devtype")
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 20
            font.family: localfont.name
            font.pixelSize: mFontSize
        }

        TextEdit {
            id: devAttr
            width: mLabelW
            height: mLabelH
            color: "#ffffff"
            text: m_pParaStyle.getLabelText("devattr")//qsTr("设备属性:")
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.top: devType.bottom
            anchors.topMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 10
            font.family: localfont.name
            font.pixelSize: mFontSize
        }

        TextEdit {
            id: devName
            width: mLabelW
            height: mLabelH
            color: "#ffffff"
            text: m_pParaStyle.getLabelText("devname")//qsTr("设备名称:")
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: devAttr.bottom
            anchors.topMargin: 20
            font.family: localfont.name
            font.pixelSize: mFontSize
        }

        TextEdit {
            id: devAddr
            width: mLabelW
            height: mLabelH
            color: "#ffffff"
            text: m_pParaStyle.getLabelText("devaddr")//qsTr("设备地址:")
            font.bold: true
            anchors.top: devName.bottom
            anchors.topMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 10
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: localfont.name
            font.pixelSize: mFontSize
        }
        TextEdit {
            id: devTypecomboBox
            height: mLabelH
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: devType.right
            anchors.leftMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 20
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: localfont.name
            font.pixelSize: mFontSize
            color: "#ffffff"
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    textType = 1
                    combox.height=0
                    comboxModel.clear()
                    var attr = m_pParaStyle.getLabelText("devattrlist")
                    console.log(attr)
                    var _attr =  attr.split(",")
                    var length = _attr.length
                    var position = 0
                    for(var i=0;i<length;i++){
                        var currentName = String(_attr.slice(i,i+1))
                        if(devTypecomboBox.text == currentName){
                            position = i
                        }
                        comboxModel.append({"_label":currentName,
                                                                 "_image":""})
                        if(combox.height < devInfo.height){
                            combox.height += mLabelH
                            console.log(combox.height)
                        }
                    }
                    combox.currentIndex = position
                    comboxModel.setProperty(position,"_image",currenImage)

                    devInfo.state = "ShowComBox"
                }
            }
        }
        TextEdit {
            id: devAttrcomboBox
            height: mLabelH
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: devAttr.right
            anchors.leftMargin: 20
            anchors.top: devTypecomboBox.bottom
            anchors.topMargin: 20
            font.family: localfont.name
            font.pixelSize: mFontSize
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: "#ffffff"
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    textType = 2
                    setDevAttrList(typeIndex,devAttrcomboBox.text)
                }
            }
        }
        TextEdit {
            id: devNameText
            height: mLabelH
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: devName.right
            anchors.leftMargin: 20
            anchors.top: devAttrcomboBox.bottom
            anchors.topMargin: 20
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: localfont.name
            font.pixelSize: mFontSize
            color: "#ffffff"
            cursorVisible: false
            text: m_pParaStyle.devName
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    m_pDeveloper.setDevName(devNameText.text)
                }
            }
        }
        TextEdit {
            id: devAddrText
            height: mLabelH
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: devAddr.right
            anchors.leftMargin: 20
            anchors.top: devNameText.bottom
            anchors.topMargin: 20
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: localfont.name
            font.pixelSize: mFontSize
            color: "#ffffff"
            cursorVisible: false
            text: m_pParaStyle.devAddr
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    m_pDeveloper.setDevAddr(devAddrText.text)
                }
            }
        }

        Button{
            id: sureBtn
            width: mLabelW
            height: mLabelH

            anchors.top: devAddrText.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            name:m_pParaStyle.getLabelText("btnname")
            fontsize: 20
            textcolor: "#000000"
            onClicked: {
                    if(devAddrText.text > 236 | devAddrText.text < 1)
                    {
                        //m_pappWindow.showToast("设备地址的范围为1~236")
                        return
                    }

                    var type = attr.split(",")
                    m_pDeveloper.saveconfigFile(
                                String(type.slice(typeIndex,typeIndex+1)),
                                devAttrcomboBox.text,
                                devNameText.text,
                                devAddrText.text)
                    devModel.setProperty(devListView.currentIndex,"_type",String(type.slice(typeIndex,typeIndex+1)))
                    devModel.setProperty(devListView.currentIndex,"_name",devNameText.text)

                    var _attr = devAttrcomboBox.text.valueOf()
                    var _addr = devAddrText.text.valueOf()
                    var idid = (_attr << 8) | _addr
                    console.log(idid,_attr,_addr,typeIndex)
                    devModel.setProperty(devListView.currentIndex,"_id",idid)
                    developer.state = ""

                }
            }
        }

        /*ListModel {
            id: devTypeModel
            ListElement {
                text:"不可调灯光"
            }
            ListElement {
                text:"可调灯光"
            }
            ListElement {
                text:"窗帘"
            }
            ListElement {
                text:"电视（红外）"
            }
            ListElement {
                text:"音乐（红外）"
            }
            ListElement {
                text:"空调（红外）"
            }
            ListElement {
                text:"自定义设备"
            }
        }
        Component {
            id: devTypecomboBoxStyle
            ComboBoxStyle{
                textColor: "#000000"
                label: Label{
                    text: devTypecomboBox.currentText
                    font.family: localfont.name
                    font.pixelSize: mFontSize
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "#000000"
                }
            }
        }
        ComboBox {
            id: devTypecomboBox
            height: mLabelH
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: devType.right
            anchors.leftMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 20
            model: devTypeModel
            style: devTypecomboBoxStyle
            onCurrentIndexChanged: {
                setDevAttrList(currentIndex,0)
            }

            //currentIndex: 5
        }

        ListModel {
            id: devAttrModel
            ListElement {
                num:0
            }
        }
        Component {
            id: devAttrcomboBoxStyle
            ComboBoxStyle{
                textColor: "#000000"
                label: Label{
                    text: devAttrcomboBox.currentText
                    font.family: localfont.name
                    font.pixelSize: mFontSize
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "#000000"
                }
            }
        }
        ComboBox {
            id: devAttrcomboBox
            height: mLabelH
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: devAttr.right
            anchors.leftMargin: 20
            anchors.top: devTypecomboBox.bottom
            anchors.topMargin: 20
            model: devAttrModel
            style:devAttrcomboBoxStyle
        }

        TextField {
            id: devNameText
            height: mLabelH
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: devName.right
            anchors.leftMargin: 20
            anchors.top: devAttrcomboBox.bottom
            anchors.topMargin: 20
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: localfont.name
            font.pixelSize: mFontSize
            style: touchStyle
        }

        TextField {
            id: devAddrText
            height: mLabelH
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: devAddr.right
            anchors.leftMargin: 20
            anchors.top: devNameText.bottom
            anchors.topMargin: 20
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.family: localfont.name
            font.pixelSize: mFontSize
            style: touchStyle

        }

        Component {
            id: touchStyle
            TextFieldStyle {
                textColor: "black"
                background: Rectangle{
                    color: "#ffffff"
                    border.color: "#06b2f4"
                    radius: 2
                    anchors.fill: parent
                }
            }
        }

        Button {
            id: sureBtn
            width: mLabelW
            height: mLabelH

            anchors.top: devAddrText.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            style: btnStyle
            onClicked: {
                if(devAddrText.text > 236 | devAddrText.text < 1)
                {
                    m_pappWindow.showToast("设备地址的范围为1~236")
                    return
                }

                var type = attr.split(",")
                m_pDeveloper.saveconfigFile(
                            String(type.slice(devTypecomboBox.currentIndex,devTypecomboBox.currentIndex+1)),
                            devAttrcomboBox.currentText,
                            devNameText.text,
                            devAddrText.text)
                devModel.setProperty(devListView.currentIndex,"_type",String(type.slice(devTypecomboBox.currentIndex,devTypecomboBox.currentIndex+1)))
                devModel.setProperty(devListView.currentIndex,"_name",devNameText.text)

                var _attr = devAttrcomboBox.currentText.valueOf()
                var _addr = devAddrText.text.valueOf()
                var idid = (_attr << 8) | _addr
                console.log(idid,_attr,_addr)
                devModel.setProperty(devListView.currentIndex,"_id",idid)
                developer.state = ""
            }
        }
        Component {
            id: btnStyle
            ButtonStyle{
                label: Label{
                    text: "确定"
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.family: localfont.name
                    font.pixelSize: mFontSize
                    color: "#000000"
                }
            }
        }*
    }*/
    Rectangle{
        id:comboxRec
        //width: parent.width
        //height: parent.height
        anchors.fill: parent
        color: "#1f000000"
        //y:-parent.height
        visible: false
        MouseArea{
            anchors.fill: parent
            onClicked: {
                devInfo.state = ""
            }
        }

        ListModel {
            id: comboxModel
            ListElement {
                _label:""
                _image:""
            }
        }
        Component {
            id: comboxDelegate
            Rectangle{
               width: combox.width
               height: mheight
               Text {
                   id: label
                   text: _label
                   font.family: localfont.name
                   font.pixelSize: mFontSize
                   //anchors.horizontalCenter: parent.horizontalCenter
                   anchors.verticalCenter: parent.verticalCenter
               }
               Image {
                   source: combox.currentIndex == index?_image:""
                   anchors.right: parent.right
                   anchors.rightMargin: 10
                   anchors.bottom: parent.bottom
                   anchors.bottomMargin: 5
                   anchors.top: parent.top
                   anchors.topMargin: 5
               }
               Rectangle{
                   width:combox.width
                   height: 1
                   color: "#000000"
                   anchors.bottom: parent.bottom
               }

               MouseArea{
                   id:comboxmouse
                   anchors.fill: parent
                   onClicked: {
                       combox.currentIndex = index

                       if(textType == 1){
                           typeIndex = index
                           devTypecomboBox.text = label.text
                       }
                       else if(textType == 2){
                           devAttrcomboBox.text = label.text
                       }
                       devInfo.state = ""
                   }
               }
            }
        }
        ListView {
            id: combox
            width: parent.width/2
            //height: 50
            //anchors.centerIn: parent.Center
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            clip: true
            delegate: comboxDelegate
            model: comboxModel
            currentIndex: -1
        }
    }

    states: [
        State {
            name: "ShowComBox"
            PropertyChanges {
                target: comboxRec
                //y:0
                visible:true
            }
        }
    ]
    transitions: Transition {
        NumberAnimation{ properties: "visible";easing.type: Easing.OutBounce;duration:2000}
    }
}


