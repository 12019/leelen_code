import QtQuick 1.0
import "roomMapToItem.js" as MapToItem

Rectangle {
    id: mainDlg
    width: 800
    height: 480
    focus: true

    property int x1:0
    property int y1:0
    property string currentimageObjectName
    property int posX: 0
    property int posY: 0
    property int fcontentX: 1000
    property int fcontentY: 1000

    /***************************************************************************************************************
      函数名：setDevInfo
      说明：修改设备的信息，地址和类型
      参数：
        oldDevId:初始的设备ＩＤ
        oldDevType:初始的设备类型
        newDevId:新的设备ＩＤ
        newDevType:新的设备类型
     ****************************************************************************************************************/
    function setDevInfo(oldDevId,oldDevType,newDevId,newDevType){
       MapToItem.setDevInfo(oldDevId,oldDevType,newDevId,newDevType)
    }

    /******************************************************************
      **********************设置转圈圈进入等待时刻************************
      ******************************************************************/
    function setLoading(true_false){
        loading.visible = true_false
    }

    /******************************************************************
      *********************根据stat打开不同的控制窗口**********************
      *****************************************************************/
    function setState(stat){
        statusBar.setHeightHide()
        if(stat == "ac"){
            objectItem.source = "AccontrolDialog.qml"
        }
        else if(stat == "music"){
            //console.log("music")
            objectItem.source = "MusicControlDialog.qml"
        }
        else if(stat == "curtain"){
            objectItem.source = "CurtainControlDialog.qml"
        }
        else if(stat == "tv"){
            objectItem.source = "TVControlDialog.qml"
        }
        else if(stat == "light"){
            objectItem.source = "LightControlDialog.qml"
        }
        else if(stat == "scene"){
            easytouch.visible = false
            statusBar.setHeightShow()
            objectItem.source = "ScenePage.qml"
        }
        else if(stat == "sceneEdit"){
            objectItem.source = "HCSceneEdit.qml"
            objectItem.item.setListModel()
            objectItem.item.clearModel()
            objectItem.item.width = mainDlg.width
            objectItem.item.height = mainDlg.height
        }
        else if(stat == "roomselect"){
            objectItem.source = "RoomSelectDialog.qml"
            objectItem.item.clearModel("lrclear")
            loading.visible = true
            objectItem.item.loadLayer()
            loading.visible = false
            objectItem.item.width = mainDlg.width
            objectItem.item.height = mainDlg.height
        }
        else if(stat == "irpasswd"){
            objectItem.source = "IrPushButtonView.qml"
            objectItem.item.width = mainDlg.width
            objectItem.item.height = mainDlg.height
        }
        else if(stat == "irlearndlg"){
            objectItem.source = "IrLearnDialog.qml"
            objectItem.item.clearModel()
            m_pIrpasswdDlg.loadIrDevice()
            objectItem.item.width = mainDlg.width
            objectItem.item.height = mainDlg.height
        }
        else if(stat == "setting"){
            objectItem.source = "HCSetting.qml"
            objectItem.item.width = mainDlg.width
            objectItem.item.height = mainDlg.height
            objectItem.item.setListModelText()
        }
        else{
            m_pappWindow.setloading(false)
            objectItem.source = ""
            easytouch.visible = true
        }
        //console.log("end")
    }

    /********************************************************************
      ****************************关闭其他窗口*****************************
      ********************************************************************/
    function closeRectangle(){
        statusBar.setHeightHide()
        objectItem.source = ""
        easytouch.visible = true
    }

    /*********************************************************************
      *************************找到设备时高亮显示***************************
      ********************************************************************/
    function setPos(x,y){
        //console.log(currentimageObjectName)
        if(currentimageObjectName == "roomImage"){
            highlight1.x = x + mainDlg.fcontentX - roomMap.contentX-25
            highlight1.y = y + mainDlg.fcontentY - roomMap.contentY-25
        }
        else if(currentimageObjectName == "roomImagePre"){
            highlight1.x = x + mainDlg.fcontentX - roomMap.contentX+roomMap.roomImageL-25
            highlight1.y = y + mainDlg.fcontentY - roomMap.contentY-25
        }
        /*else if(currentimageObjectName == "roomImageNext"){
            highlight.x = x + 1000 - roomMap.contentX-roomMap.roomImageL-25
            highlight.y = y + 1000 - roomMap.contentY-25
        }*/
        action1.start()
        //action2.start()
    }

    /***************************************************************
    ***************************载入家居图片**************************
    ****************************************************************/
    function setRoomImage(image){
        //console.log(image)
        roomMap.contentX = fcontentX
        roomMap.contentY = fcontentY
        roomMap.roomImage = image
    }

    /****************************************************************
    ****************************增加item*****************************
    ****************************************************************/
    function addItem(xPos,yPos,id,type){
        MapToItem.addDevItem(xPos+mainDlg.fcontentX,yPos+mainDlg.fcontentY,id,type)
        MapToItem.addDevItem(xPos+roomMap.roomImageL+mainDlg.fcontentX,yPos+mainDlg.fcontentY,id,type)
        //MapToItem.handleClick(xPos-roomMap.roomImageL+1000,yPos+1000,id,type)
    }

    /*****************************************************************
      *************************清除所有item*****************************
      *****************************************************************/
    function clearAllItem(){
        MapToItem.removeAll()
    }


    /**********************************************************************
    ********************************显示状态图标****************************
    ********************************type:属性类型****************************
    ********************************id:设备的id******************************
    ***********************************************************************/
    function showDevIcon(type,id){
        MapToItem.showDevIcon(type,id)
    }

    /**********************************************************************
    ********************************隐藏状态图标****************************
    ********************************type:属性类型****************************
    ********************************id:设备的id******************************
    ***********************************************************************/
    function hideDevIcon(type,id){
        MapToItem.hideDevIcon(type,id)
    }

    /******************************************************
      *********************房间选择调用函数******************
      *****************************************************/
    function roomselectEvent(type,name){
        if(type == "lclear" | type == "rclear" | type == "lrclear"){
            objectItem.item.clearModel(type)
        }
        else if(type == "loadLayer"){
            objectItem.item.loadLayer()
        }
        else{
            objectItem.item.addModel(type,name)
        }
    }

    function addRoomModel(name,image){
        if(objectItem.item.objectName == "roomselect")
            objectItem.item.addRoomModel(name,image)
    }

    /*****************************************************************************
    ******************************主函数访问场景编辑的函数****************************
    ******************************************************************************/
       function hcsceneeditEvent(typename,name,power,deviceid,type,active){
           if(typename == "sceneModel")
               objectItem.item.setListModel()
           else if(typename == "actionModel")
               objectItem.item.setView(name,power,deviceid,type,active)
           else if(typename == "clearModel"){
               objectItem.item.clearModel()
           }
           else if(typename>0 && typename<9){
               objectItem.item.irDecEdit(typename)
           }
       }
       function lightresetStat(lighten){
           objectItem.item.lightresetStat(lighten)
       }
       function tvresetStat(mode){
           objectItem.item.tvresetStat(mode)
       }
       function setSceneViewName(index,name,newname){
           objectItem.item.setSceneViewName(index,name,newname)
       }

       /*********************************************************/
       /***********************红外学习**************************/
       /********************************************************/
       function irLearnEvent(type,name,id){
           objectItem.item.addModel(type,name,id)
       }
       function irLearnDone(){
           objectItem.item.setModelPic()
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

           if(objectItem.item.objectName == "setting"){
               objectItem.item.setDevListView(_name,_id,_type)
           }
       }

       /***********************************************************************************
       函数名 :settingsetState
       描  述 : 调用设置界面的函数，stat为要载入的qml文件
       输入参数说明 :
       输出参数说明 :
       返 回 值:
       其它说明:
       ***********************************************************************************/
       function settingsetState(stat){
           if(objectItem.item.objectName == "setting"){
               objectItem.item.setState(stat)
           }
       }

    /**************************************************************
      ************************主界面视图*****************************
      **************************************************************/
    Flickable {
        id:roomMap
        property int flag: 0
        property int num: 0
        property string roomImage
        property real roomImageL:roomImage.width
        property real roomImageH:roomImage.height
        property real conY1
        property real conY2
        property real conX1
        property real scaleNum
        anchors.fill: parent
        //flickDeceleration: 1500
        //maximumFlickVelocity: 1500
        clip: true
        contentWidth: 10000//mainDlg.fcontentWidth
        contentHeight: 10000//mainDlg.fcontentHeight
        contentX: 1000//mainDlg.fcontentX
        contentY: 1000//mainDlg.fcontentY
        focus: true

        onContentXChanged: {
            if(roomMap.contentX > mainDlg.fcontentX+roomImageL)
            {
                roomMap.contentX=mainDlg.fcontentX
            }
            else if(roomMap.contentX < mainDlg.fcontentX){
                roomMap.contentX=mainDlg.fcontentX+roomImageL
            }
        }
        onContentYChanged: {
            //roomMap.flag = 0
            if(roomMap.contentY <= mainDlg.fcontentY)
                roomMap.contentY = mainDlg.fcontentY
            if(roomMap.contentY >= mainDlg.fcontentY+roomImageH-m_pappWindow.screenheight())
                roomMap.contentY = mainDlg.fcontentY+roomImageH-m_pappWindow.screenheight()
        }
        onFlickStarted: {
            //console.log("flick started")
        }
        onMovementStarted: {
            roomMap.flag = 0
            //console.log("move started")
        }

        BorderImage {
            id: roomImagePre
            objectName: "roomImagePre"
            source: roomMap.roomImage
            x:roomImage.width+mainDlg.fcontentX
            y:mainDlg.fcontentY
            MouseArea{

                anchors.fill: parent
                onClicked: {
                    //console.log("click",mouseX,mouseY)
                    m_pappWindow.findDevice(mainDlg.x1,mainDlg.y1)
                }
                onPressed: {
                    roomMap.flag = 1
                    mainDlg.x1 = mouseX
                    mainDlg.y1 = mouseY
                    //console.log(roomImagePre.objectName,mouseX,mouseY)
                    closeRectangle()
                    currentimageObjectName = roomImagePre.objectName
                }
                onReleased: {
                    //console.log("release")
                }
                onExited: {
                    //console.log("exit")
                    x= - 1
                    y = -1
                }
                onCanceled: {
                    //console.log("cancel",mouseX,mouseY)
                    if(roomMap.flag == 1){
                        m_pappWindow.findDevice(mainDlg.x1,mainDlg.y1)
                    }
                    roomMap.flag = 1
                }
            }
        }
        BorderImage {
            id: roomImage
            objectName: "roomImage"
            source: roomMap.roomImage
            x:fcontentX
            y:fcontentY
            focus: true
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    //console.log("click")
                    m_pappWindow.findDevice(mainDlg.x1,mainDlg.y1)
                }
                onPressed: {
                    roomMap.flag = 1
                    //console.log(roomImage.objectName)
                    closeRectangle()
                    currentimageObjectName = roomImage.objectName
                    mainDlg.x1 = mouseX
                    mainDlg.y1 = mouseY
                }
                onReleased: {
                    //console.log("release")
                }
                onExited: {
                    //console.log("exit")
                }
                onCanceled: {
                    //console.log("cancel")
                    if(roomMap.flag == 1){
                        m_pappWindow.findDevice(mainDlg.x1,mainDlg.y1)
                    }
                    roomMap.flag = 1
                }
            }
        }

    }


    /*********************************************************************
      **************************显示高亮的白圈******************************
      *********************************************************************/
    Rectangle {
        id: highlight1
        width: 50
        height: 50
        color: "#a0ffffff"
        radius: 35
        opacity: 0
        x:-500
        y:-500
    }


    /**********************************************************************
      **************************easytocuh按钮可滑动***************************
      **********************************************************************/
    EasyTouch{
        id:easytouch
        x:(m_pappWindow.screenwidth() - 200)
        y:(m_pappWindow.screenheight() - 200)
        onClicked: {
            easytouch.visible = false
            m_pappWindow.setMainState("scene")
        }
        onXChanged: {
            if(x <= 0){
                easytouch.x = 0
            }
            else if(x >= m_pappWindow.screenwidth() - width){
                easytouch.x = m_pappWindow.screenwidth() - width
            }
        }
        onYChanged: {
            if(y <= statusBar.height){
                easytouch.y = statusBar.height
            }
            else if(y >= m_pappWindow.screenheight() - height){
                easytouch.y = m_pappWindow.screenheight() - height
            }
        }
    }

    /**********************************************************************
      ******************************状态栏**********************************
      **********************************************************************/
    HCStatusBar{
        id:statusBar
        objectName: "statusBar"
        x: 0
        y: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
    }

    Loader{
        id:objectItem
        anchors.centerIn: parent
        onStatusChanged: {
            if(objectItem.status == Loader.Ready){
                //console.log("loaded")
            }
            else if(objectItem.status == Loader.Loading){
                //console.log("loading")
            }
            else if(objectItem.status == Loader.Null){
                //console.log("null")
            }
        }
    }

    Loading{
        id:loading
        anchors.centerIn: parent
        visible: false
    }



    /**********************************************************************
      ********************一下两个函数为显示高亮的效果************************
      **********************************************************************/
    PropertyAnimation{
        id:action1
        target: highlight1
        property: "opacity"
        from:0
        to:1
        duration: 10

        onCompleted: {
            action11.start()
        }
    }
    PropertyAnimation{
        id:action11
        target: highlight1
        property: "opacity"
        from:1
        to:0
        duration: 800
        onCompleted:  {
            highlight1.x = -50
            highlight1.y = -50
        }
    }
}
