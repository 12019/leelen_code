import QtQuick 1.0

Rectangle {
    id: sceneEdit
    width: 800
    height: 480
    color: "#ffffff"

    property string iconOff : "images/home_control/list/off.png"
    property string iconOn : "images/home_control/list/on.png"
    property string qichuangmoshi : "images/home_control/list/0"        //起床模式
    property string jiucanmoshi : "images/home_control/list/1"             //就餐模式
    property string yingyinmoshi : "images/home_control/list/2"            //影音模式
    property string shuimianmoshi : "images/home_control/list/3"       //睡眠模式
    property string zidingyiA : "images/home_control/list/4"                   //自定义A
    property string zidingyiB : "images/home_control/list/5"                   //自定义B
    property string quxiao : "images/home_control/list/quxiao.png"       //取消
    property string quxiao_1 : "images/home_control/list/quxiao-1.png"   //
    property string wancheng : "images/home_control/list/wancheng.png"
    property string wancheng_1 : "images/home_control/list/wancheng-1.png"
    property string quanguan : "images/home_control/scene/quanguan.png"
    property string quanguan_1 : "images/home_control/scene/quanguan-1.png"
    property string quankai : "images/home_control/scene/quankai.png"
    property string quankai_1 : "images/home_control/scene/quankai-1.png"

    //property string iconOff: "images/home_control/list/off.png"
    //property string iconOn: "images/home_control/list/on.png"
    property int i: 0
    property string flag

    LocalFontLoader{
        id:localfont
    }

    MouseArea{
        anchors.fill: parent
    }

    function clearModel(){
        actionModel.clear()
    }

    function setView(name,power,deviceid,type,active){
        actionModel.append({
                       "name": name,
                       "power":power,
                       "deviceid":deviceid,
                       "type":type,
                        "activeflag":active})
    }

    function setListModel(){
        listModel.clear()
        listModel.append({
                   "name":m_pHCSceneDialog.s1text, //"起床模式",
                   "picture":qichuangmoshi})
        listModel.append({
                   "name": m_pHCSceneDialog.s2text, //"就餐模式",
                   "picture":jiucanmoshi})
        listModel.append({
                   "name": m_pHCSceneDialog.s3text, //"影音模式",
                   "picture":yingyinmoshi})
        listModel.append({
                   "name": m_pHCSceneDialog.s4text, //"睡眠模式",
                   "picture":shuimianmoshi})
        listModel.append({
                   "name": m_pHCSceneDialog.s5text, //"自定义A",
                   "picture":zidingyiA})
        listModel.append({
                   "name": m_pHCSceneDialog.s6text, //"自定义B",
                   "picture":zidingyiB})
        sceneview.currentIndex = -1

    }

    function setSceneViewName(index,name,newname){
        listModel.setProperty(index,name,newname)
    }

    function irDecEdit(type){
        switch(type)
        {
            case 1:
            lightEdit.opacity = 1
            tvEdit.opacity = 0
            musicEdit.opacity = 0
            acEdit.opacity = 0
            break;
            case 4:
            lightEdit.opacity = 0
            tvEdit.opacity = 0
            musicEdit.opacity = 0
            acEdit.opacity = 1
            break;
            case 5:
            lightEdit.opacity = 0
            tvEdit.opacity = 1
            musicEdit.opacity = 0
            acEdit.opacity = 0
            break;
            case 6:
            lightEdit.opacity = 0
            tvEdit.opacity = 0
            musicEdit.opacity = 1
            acEdit.opacity = 0
            break;
        }
    }

    function hideIrDec(){
        lightEdit.opacity = 0
        tvEdit.opacity = 0
        musicEdit.opacity = 0
        acEdit.opacity = 0
    }

    property string light
    property string tvMode

    function lightresetStat(lighten){
        light = lighten
    }
    function tvresetStat(mode){
        tvMode = mode
    }

    property bool ispress: false
    property bool isnext: false
    Timer{
        id:timer1
        interval: 800
        running: false
        repeat: false
        onTriggered: {
            timer2.start()
        }
    }

    Timer {
    id:timer2
    interval: 200
    running: false;
    repeat: true
    onTriggered: {
            if(!isnext){
                if(Number(light)>0){
                    if(Number(light) - 1 <= 0)
                        light = "0"
                    else
                        light=String(Number(light)-1);

                    m_pHCSceneEdit.irDevEidt("value",light)
                }
            }
            else{
                if(Number(light)<100){
                    if(Number(light) + 1 >= 100)
                        light = "100"
                    else
                        light=String(Number(light)+1);
                    m_pHCSceneEdit.irDevEidt("value",light)
                }
            }
        }
    }

    Rectangle{
        x: 235
        y: 39
        color: "#efefef"
        anchors.left: sceneview.right
        anchors.leftMargin: -20
        //
        anchors.top: sceneBar.bottom
        anchors.topMargin: 0
        width: actionview.width+40
        height: actionview.height
    }


    ListModel {
        id: listModel
        ListElement {
            name: "起床模式"
            picture:"images/home_control/list/0"
            //picture:qichuangmoshi
        }
    }
    Component {
        id: recipeDelegate
        Rectangle {
            id: recipe
            height: 147//sceneview.height/3
            width: 255
            color: "#00000000"
            Image {
                id: recipeImage
                anchors.fill: parent
                source: sceneview.currentIndex == index?
                            (picture+"-"+String(index)+".png"):
                            (picture+".png")
            }
            TextEdit {
                id:sceneViewName
                text: name
                font.bold: true
                width: recipe.width
                height: recipe.height/4
                y:100
                font.pixelSize: 20
                font.family: localfont.name
                color: "#000000"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                cursorVisible: false
                MouseArea{
                    anchors.fill: parent
                    onPressed: {
                        sceneview.flag = 1
                        console.log("pressed")
                    }
                    onCanceled: {
                        console.log("oncanceled")
                        if(sceneview.flag == 1){
                            if(index == 0)
                            {
                                m_pHCSceneDialog.pushBtnScene1Clicked(sceneViewName.text)
                                //listModel.setProperty(index,"name",m_pHCSceneDialog.s1text)
                            }
                            else if(index == 1)
                            {
                                m_pHCSceneDialog.pushBtnScene2Clicked(sceneViewName.text)
                                //listModel.setProperty(index,"name",m_pHCSceneDialog.s2text)
                            }
                            else if(index == 2)
                            {
                                m_pHCSceneDialog.pushBtnScene3Clicked(sceneViewName.text)
                                //listModel.setProperty(index,"name",m_pHCSceneDialog.s3text)
                            }
                            else if(index == 3)
                            {
                                m_pHCSceneDialog.pushBtnScene4Clicked(sceneViewName.text)
                                //listModel.setProperty(index,"name",m_pHCSceneDialog.s4text)
                            }
                            else if(index == 4)
                            {
                                m_pHCSceneDialog.pushBtnScene5Clicked(sceneViewName.text)
                                //listModel.setProperty(index,"name",m_pHCSceneDialog.s5text)
                            }
                            else if(index == 5)
                            {
                                m_pHCSceneDialog.pushBtnScene6Clicked(sceneViewName.text)
                                //listModel.setProperty(index,"name",m_pHCSceneDialog.s6text)
                            }
                        }
                        sceneview.flag = 1
                    }

                    onClicked: {
                        console.log(index)
                        if(index == 0)
                        {
                            m_pHCSceneDialog.pushBtnScene1Clicked(sceneViewName.text)
                            //listModel.setProperty(index,"name",m_pHCSceneDialog.s1text)
                        }
                        else if(index == 1)
                        {
                            m_pHCSceneDialog.pushBtnScene2Clicked(sceneViewName.text)
                            //listModel.setProperty(index,"name",m_pHCSceneDialog.s2text)
                        }
                        else if(index == 2)
                        {
                            m_pHCSceneDialog.pushBtnScene3Clicked(sceneViewName.text)
                            //listModel.setProperty(index,"name",m_pHCSceneDialog.s3text)
                        }
                        else if(index == 3)
                        {
                            m_pHCSceneDialog.pushBtnScene4Clicked(sceneViewName.text)
                            //listModel.setProperty(index,"name",m_pHCSceneDialog.s4text)
                        }
                        else if(index == 4)
                        {
                            m_pHCSceneDialog.pushBtnScene5Clicked(sceneViewName.text)
                            //listModel.setProperty(index,"name",m_pHCSceneDialog.s5text)
                        }
                        else if(index == 5)
                        {
                            m_pHCSceneDialog.pushBtnScene6Clicked(sceneViewName.text)
                            //listModel.setProperty(index,"name",m_pHCSceneDialog.s6text)
                        }
                    }
                    onReleased: {
                    }
                }
            }


            MouseArea{
                id:sceneviewmouse
                anchors.bottom: sceneViewName.top
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottomMargin: 0

                onPressed: {
                    sceneview.flag = 1
                    console.log("pressed")
                }
                onCanceled: {
                    console.log("oncanceled")
                    if(sceneview.flag == 1){
                        if(lightEdit.opacity == 1 | tvEdit.opacity == 1 | musicEdit.opacity == 1 | acEdit.opacity == 1){
                            hideIrDec()
                            return
                        }
                        sceneview.currentIndex = index
                        actionModel.clear()
                        if(m_pHCSceneEdit.loadDevice(index) == false)
                            m_pappWindow.setloading(false)
                    }
                    sceneview.flag = 1
                }

                onClicked: {
                    console.log("click")
                    sceneview.currentIndex = index
                }
                onReleased: {
                    if(lightEdit.opacity == 1 | tvEdit.opacity == 1 | musicEdit.opacity == 1 | acEdit.opacity == 1){
                        hideIrDec()
                        return
                    }
                    console.log("release")
                    actionModel.clear()
                    if(m_pHCSceneEdit.loadDevice(index) == false)
                        m_pappWindow.setloading(false)
                }
            }
        }
    }

    ListView
    {
        property int flag: 1
        ScrollBar
        {
            scrollArea: sceneview
            height: sceneview.height
            width: 10
            x:sceneview.width-25
        }
        id: sceneview   //场景
        width: 255
        highlightMoveSpeed: 999
        highlightMoveDuration: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        maximumFlickVelocity: 2500
        cacheBuffer: 0
        anchors.left: parent.left
        anchors.leftMargin: 0

        currentIndex: -1
        anchors.top: sceneBar.bottom
        anchors.topMargin: 0
        clip: true
        model: listModel
        delegate: recipeDelegate
        onMovementStarted: {
            console.log("move start")
            flag = 0
        }
    }

    ListModel {
        id: actionModel
        ListElement {
            name: ""
            power:""
            deviceid:""
            type:""
            activeflag:"disable"
        }
    }

    Component {
        id: actionDelegate

        Item {
            id: action
            height: 100//actionview.height/5
            width: actionview.width
            BorderImage {
                id: actionOff
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                source: power//"images/home_control/list/off.png"
                visible: true
            }
            Text {
                text: name
                anchors.verticalCenter: parent.verticalCenter
                x:20
                font.bold: true
                font.family: localfont.name
                font.pixelSize: 20
                color: "#000000"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                id: d_id
                text: deviceid
                visible: false
            }
            Text {
                id: d_type
                text: type
                visible: false
            }
            Rectangle{
                id:active
                anchors.centerIn: parent
                width: activeLabel.width+20
                height: activeLabel.height+20
                color: activeflag == "enable"?"#05f188":"#e40410"
                radius: 10

                Text {
                    id:activeLabel
                    text: m_pHCSceneEdit.isEnable(activeflag)//activeflag == "enable"?"已激活":"未激活"
                    visible: true
                    font.bold: true
                    font.family: localfont.name
                    font.pixelSize: 20
                    color: "#000000"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    anchors.centerIn: parent
                }
            }

            Rectangle{
                x:4
                width:actionview.width
                height: 1
                color: "#8f8f8f"
                anchors.bottom: parent.bottom
            }


            MouseArea{
                anchors.right: actionOff.left
                anchors.rightMargin:0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0

                /*onPressed: {
                    sceneview.flag = 1
                }
                onCanceled: {
                    if(sceneview.flag == 1){

                    }
                }*/

                onClicked: {
                    actionview.currentIndex = index
                }
                onReleased: {
                    if(mouseX >= active.x &&
                            mouseX <= active.x+active.width &&
                            mouseY >= active.y &&
                            mouseY <= active.y+active.height){
                        if(/*activeLabel.text == "已激活"*/activeflag == "enable"){
                            actionModel.setProperty(index,"activeflag","disable")
                            active.color = "#e40410"
                            m_pHCSceneEdit.eidtDevice(true,d_id.text,d_type.text,false,"disable")
                            activeLabel.text = m_pHCSceneEdit.isEnable("disable")
                        }
                        else{
                            actionModel.setProperty(index,"activeflag","enable")
                            active.color = "#05f188"
                            m_pHCSceneEdit.eidtDevice(true,d_id.text,d_type.text,false,"enable")
                            activeLabel.text = m_pHCSceneEdit.isEnable("enable")
                        }
                        return
                    }

                    if(lightEdit.opacity == 1 | tvEdit.opacity == 1 | musicEdit.opacity == 1 | acEdit.opacity == 1){
                        hideIrDec()
                        return
                    }
                    actionview.currentIndex = index
                    m_pHCSceneEdit.eidtDevice(true,d_id.text,d_type.text,false,"")
                }
            }




            MouseArea{
                anchors.fill: actionOff
                onClicked: {
                    actionview.currentIndex = index
                }
                onReleased: {
                    actionview.currentIndex = index
                    console.log(actionOff.source)
                    if(actionOff.source == "qrc:/Myqml/" + iconOff)
                    {
                        actionModel.setProperty(index,"power",iconOn)
                        m_pHCSceneEdit.eidtDevice(true,d_id.text,d_type.text,true,"")
                    }
                    else if(actionOff.source == "qrc:/Myqml/" +  iconOn)
                    {
                        actionModel.setProperty(index,"power",iconOff)
                        m_pHCSceneEdit.eidtDevice(false,d_id.text,d_type.text,true,"")
                    }
                }
            }
        }
    }

    ListView
    {
        ScrollBar
        {
            scrollArea: actionview
            height: actionview.height
            width: 10
            x:actionview.width-25
        }
        id: actionview
        highlightMoveDuration: 0  //设备编辑
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        cacheBuffer: 0
        //flickDeceleration: 5000
        //maximumFlickVelocity: 500
        anchors.left: sceneview.right
        anchors.leftMargin: 0
        anchors.top: sceneBar.bottom
        anchors.topMargin: 0
        clip: true
        model: actionModel
        delegate: actionDelegate
        onCurrentSectionChanged: {
            //scrollbar1.visible = true
        }
        highlight: Rectangle{
            width: actionview.width
            color: "#b4b4b4"
        }
        highlightMoveSpeed: 999
    }

    Rectangle {
        id: sceneBar
        height: 39
        color: "#000000"
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        Button{
            height: 27
            width: 75
            color:"#00000000"
            anchors.left: parent.left
            anchors.leftMargin: 10
            image:quxiao//"images/home_control/list/quxiao.png"
            imagepressed: quxiao_1//"images/home_control/list/quxiao-1.png"
            anchors.verticalCenter: parent.verticalCenter
            textcolor: "#ffffff";
            fontsize: 18
            name: "     "+m_pHCSceneEdit.cancel
            onClicked: {
                actionModel.clear()
                actionview.currentIndex = -1
                m_pHCSceneEdit.close()
                hideIrDec()
            }
        }
        Button{
            height: 27
            width: 75
            color:"#00000000"
            anchors.right: parent.right
            anchors.rightMargin: 10
            image:wancheng//"images/home_control/list/wancheng.png"
            imagepressed: wancheng_1//"images/home_control/list/wancheng-1.png"
            anchors.verticalCenter: parent.verticalCenter
            textcolor: "#ffffff";
            fontsize: 18
            name: "     "+m_pHCSceneEdit.finish
            onClicked: {
                m_pHCSceneEdit.editFinish()
                actionModel.clear()
                sceneview.currentIndex = -1
                hideIrDec()
            }
        }

        Button {
            id: alloff
            width: 32
            height: 32
            image: quanguan//"images/home_control/scene/quanguan.png"
            anchors.verticalCenter: parent.verticalCenter
            color: "#00000000"
            anchors.left: parent.left
            anchors.leftMargin: 305
            imagepressed: quanguan_1//"images/home_control/scene/quanguan-1.png"
            onClicked: {
                for(i=0;i<actionModel.count;i++){
                    actionModel.setProperty(i,"power",iconOff)
                }
                m_pHCSceneEdit.setDevAllOff()
            }
        }
        Button {
            id: allon
            width: 32
            height: 32
            imagepressed: quankai_1//"images/home_control/scene/quankai-1.png"
            image: quankai//"images/home_control/scene/quankai.png"
            anchors.right: parent.right
            anchors.rightMargin: 305
            anchors.verticalCenter: parent.verticalCenter
            color: "#00000000"
            onClicked: {
                for(i=0;i<actionModel.count;i++){
                    actionModel.setProperty(i,"power",iconOn)
                }
                m_pHCSceneEdit.setDevAllOn()
            }
        }
    }

    Rectangle{    //空调编辑
        id:acEdit
        width: 436
        height: 164
        opacity: 0
        radius: 10
        anchors.centerIn: parent
        DeviceButton{ //固定、摆风
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            msg:m_pHCSceneEdit.swing
            onNextclicked: m_pHCSceneEdit.pushButtonACSwing(msg)
            onPreclicked: m_pHCSceneEdit.pushButtonACSwing(msg)
        }
        DeviceButton { //制冷、加热、通风、除湿
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 10
            msg:m_pHCSceneEdit.mode
            onPreclicked: m_pHCSceneEdit.pushButtonACMode("pre")
            onNextclicked:m_pHCSceneEdit.pushButtonACMode("next")
        }
        DeviceButton { //自动、低风、中风、高风
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            msg:m_pHCSceneEdit.wind
            onNextclicked: m_pHCSceneEdit.pushButtonACWind("next")
            onPreclicked: m_pHCSceneEdit.pushButtonACWind("pre")
        }
        DeviceButton {  //温度
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            msg:m_pHCSceneEdit.temp+"℃"
            onPreclicked: m_pHCSceneEdit.pushButtonACTemperature("pre")
            onNextclicked: m_pHCSceneEdit.pushButtonACTemperature("next")
        }
    }
    Rectangle{
        id:lightEdit
        width: 238
        height: 124
        radius: 10
        opacity: 0
        anchors.centerIn: parent
        DeviceButton {//亮度
            id: devicebutton1
            anchors.centerIn: parent
            msg:light+"%"

            onNextPressed: {
                isnext=true
                timer1.start()
            }
            onPrePressed: {
                isnext=false
                timer1.start()
            }
            onButtonReleased: {
                timer1.stop()
                timer2.stop()
                if(!isnext){
                    if(Number(light)>0){
                        light=String(Number(light)-1);
                        m_pHCSceneEdit.irDevEidt("value",light)
                    }
                }
                else{
                    if(Number(light)<100){
                        light=String(Number(light)+1);
                        m_pHCSceneEdit.irDevEidt("value",light)
                    }
                }

            }
        }
    }

    Rectangle{   //北京音乐编辑
        id:musicEdit
        width: 268
        height: 146
        radius: 10
        opacity: 0
        anchors.centerIn: parent

        DeviceButton {//静音、非静音
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 10
            msg:m_pHCSceneEdit.mute
            onPreclicked: m_pHCSceneEdit.pushButtonMusicMute(msg)
            onNextclicked: m_pHCSceneEdit.pushButtonMusicMute(msg)
        }
        DeviceButton {//MP3、aux、cd、radio
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            msg:m_pHCSceneEdit.source
            onPreclicked:m_pHCSceneEdit.pushButtonBMSource("pre")
            onNextclicked: m_pHCSceneEdit.pushButtonBMSource("next")
        }
    }

    Rectangle{    //电视编辑
        id:tvEdit
        height: 134
        radius: 10
        width: 248
        anchors.centerIn: parent
        opacity: 0

        DeviceButton {
            id: devicebutton2
            anchors.centerIn: parent
            msg:tvMode
            onNextclicked: {
                if(tvMode == "tv")
                    tvMode = "av"
                else
                    tvMode = "tv"
                m_pHCSceneEdit.irDevEidt("mode",tvMode)
            }
            onPreclicked: {
                if(tvMode == "tv")
                    tvMode = "av"
                else
                    tvMode = "tv"
                m_pHCSceneEdit.irDevEidt("mode",tvMode)
            }
        }
    }
}
























