import   QtQuick 1.0
import   LuxDomoWLANSetting  1.0
import    "../content"
Rectangle
{
    id:wlanSetting
    objectName: "wlanSetting"
    width: 450
    height: 480
    color:"#efeff4"

    property    bool     isWlan: false
    property    bool     inputPasswdEnable: false
    property    bool     optionEnable: false
    property    bool     optionListEnable: false
    property    bool     parameterSettingsEnable: false

    property    string   pressBssid: ""
    property    string   pressSsid:""
    property    string   pressPsk:""
    property    string   pressFlags: ""
    property    string   pressRange: ""
    property    string   pressSignal:""
    property    int        pressIndex: -1

    property    string   pressIp:""
    property    string   pressGateway:""
    property    string   pressNetmask:""
    property    string   pressDns:""

    property    string   ssid:""
    property    string   psk:""
    property    string   flags:""
    property    string   range:""
    property    string   sig: ""
    property    string   bssid: ""
    property    string   condition:""


    function    clearModel()      {  model.clear()  }
    function    changModel(index,propertyName,propertyValue){  model.setProperty( index,propertyName,propertyValue) }
    function    removeModel(index){  model.remove(index)  }
    function    addModel(listBssid,listSsid,listPsk,listFlags,listRange,listSignal,listImage)
    {
        model.append( { "listBssid":listBssid,"listSsid":listSsid,"listPsk":listPsk,"listFlags":listFlags,
                                     "listRange":listRange,"listSignal":listSignal,"listImage":listImage} )
    }
    function    insertModel(index,listBssid,listSsid,listPsk,listFlags,listRange,listSignal,listImage)
    {
        model.insert( index,{ "listBssid":listBssid,"listSsid":listSsid,"listPsk":listPsk,"listFlags":listFlags,
                                            "listRange":listRange,"listSignal":listSignal,"listImage":listImage})
    }

    function    init(){  pLuxDomoWLANSettingItem.init() }
    function   setInformation(data){ pLuxDomoWLANSettingItem.setInformation(data) }

    function    close()
    {
        optionEnable=false
        optionListEnable=false
        inputPasswdEnable=false
        parameterSettingsEnable=false

        pressBssid=""
        pressSsid=""
        pressPsk=""
        pressFlags=""
        pressRange= ""
        pressSignal=""
        pressIndex=-1
        pressDns=""
        pressGateway=""
        pressIp=""
        pressNetmask=""
    }

    LuxDomoWLANSettingItem
    {
        id:pLuxDomoWLANSettingItem
        objectName: "pLuxDomoWLANSettingItem"
    }

    MouseArea{    anchors.fill: parent    }

    Rectangle
    {
        width: parent.width
        height: parent.height/8
        color:"#f6f6f7"
        Rectangle
        {
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
            color: "#bfbfbf"
        }
        LuxDomoLabel
        {
            anchors.fill: parent
            textName: "WLAN设置"
            textSize: 25
        }
    }

    Item
    {
        width: parent.width
        height: parent.height*2/8
        anchors.top: parent.top
        anchors.topMargin: parent.height/8
        Column
        {
            anchors.fill: parent
            LuxDomoLabel1
            {
                width: parent.width
                height: parent.height/4
                margin: parent.width/12
                textName: "开关"
                textSize: 16
            }
            Rectangle
            {
                width: parent.width
                height: 1
                color: "#bfbfbf"
            }
            Item
            {
                width: parent.width
                height: parent.height/2-3
                Row
                {
                    width: parent.width*11/12
                    height: parent.height
                    anchors.right: parent.right
                    Item
                    {
                        width: parent.width/12
                        height: parent.height
                        Image
                        {
                            width: parent.width*9/10
                            height:parent.width*9/10
                            anchors.verticalCenter: parent.verticalCenter
                            source: "../setting/wifi_small.png"
                        }
                    }
                    LuxDomoLabel
                    {
                        width: parent.width/4
                        height: parent.height
                        textName: "WI_FI"
                        textSize: 20
                    }
                    Item
                    {
                        width: parent.width*2/3
                        height: parent.height
                        Image
                        {
                            width: parent.width/5
                            height: parent.height/2
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            anchors.rightMargin: parent.width/15
                            source: isWlan?"../setting/on_off_on.png":"../setting/on_off_off.png"
                        }
                    }
                }
                MouseArea
                {
                    anchors.fill: parent
                    onClicked: {  pLuxDomoWLANSettingItem.on_offWlan(isWlan)  }
                }
            }
            Rectangle
            {
                width: parent.width
                height: 1
                color: "#bfbfbf"
            }
            LuxDomoLabel1
            {
                width: parent.width
                height: parent.height/4
                margin: parent.width/12
                visible: isWlan
                textName: "可用WLAN列表"
                textSize: 16
            }
            Rectangle
            {
                width: parent.width
                visible: isWlan
                height: 1
                color: "#bfbfbf"
            }
        }
    }

    ListModel
    {
        id: model
        ListElement
        {
            listBssid: ""
            listSsid:""
            listPsk:""
            listFlags: ""
            listRange:""
            listSignal:""
            listImage:""
        }
    }

    Component
     {
         id:delegate
         Rectangle
         {
             id:dataItem
             width: list.width
             height: list.height/4
             Rectangle
             {
                 width: parent.width
                 height: 1
                 anchors.bottom: parent.bottom
                 anchors.right: parent.right
                 color: "#bfbfbf"
             }
             LuxDomoLabel1
             {
                 width: parent.width*3/4
                 height: parent.height
                 anchors.left:parent.left
                 anchors.leftMargin: parent.width/12
                 textName: listSsid
                 textSize: 20
             }
             Item
             {
                 width: parent.width/6
                 height: parent.height
                 anchors.right: parent.right
                 visible: listRange=="on"?true:false
                 Image
                 {
                     height: parent.height*3/5
                     width: parent.height*3/5
                     anchors.verticalCenter:parent.verticalCenter
                     anchors.horizontalCenter:parent.horizontalCenter
                     source: listImage
                 }
             }
             MouseArea
             {
                 anchors.fill: parent
                 onClicked:
                 {
                     if(listRange=="on")
                     {
                         inputPasswdEnable =true
                         pressIndex=index;
                         pressSsid=listSsid
                         pressPsk=listPsk
                        pressRange=listRange
                        pressFlags=listFlags
                        pressSignal=listSignal
                        pressBssid=listBssid
                     }
                 }
                 onPressAndHold:
                 {
                     optionListEnable=true
                     pressIndex=index;
                     pressSsid=listSsid
                     pressPsk=listPsk
                    pressRange=listRange
                    pressFlags=listFlags
                    pressSignal=listSignal
                    pressBssid=listBssid
                 }
             }
         }
    }

    ListView
    {
        id: list
        width: parent.width
        height: parent.height*4/10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/8
        anchors.horizontalCenter: parent.horizontalCenter
        clip: true
        model: model
        delegate:delegate
        visible: isWlan
     }

    Rectangle
    {
        width: parent.width
        height: parent.height/10
        anchors.bottom: list.top
        visible: isWlan
        LuxDomoLabel1
        {
            width: parent.width*3/4
            height: parent.height
            anchors.left:parent.left
            anchors.leftMargin: parent.width/12
            textName:ssid
            textSize: 20
        }
        LuxDomoLabel
        {
            width: parent.width/4
            height: parent.height
            anchors.right: parent.right
            textName:condition
            textSize: 20
        }
        Rectangle
        {
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            color: "#bfbfbf"
        }
        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                if(ssid!="")
                {
                    optionEnable=true
                }
            }
        }
    }

    Rectangle
    {
        width:parent.width
        height: parent.height/8
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        color: "#bfbfcf"
        visible: isWlan
        LuxDomoLabel
        {
            width: parent.width/2-1
            height: parent.height
            anchors.left: parent.left
            textName: "扫描"
            textSize: 20
            MouseArea
            {
                anchors.fill: parent
                onClicked: {pLuxDomoWLANSettingItem.scanWLAN() }
            }
        }
        Rectangle
        {
            width: 2
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: "white"
        }
        LuxDomoLabel
        {
            width: parent.width/2-1
            height: parent.height
            anchors.right: parent.right
            textName: "自动连接"
            textSize: 20
            MouseArea
            {
                anchors.fill: parent
                onClicked: {  pLuxDomoWLANSettingItem.smartLinkWLAN() }
            }
        }
    }

    Item
    {
        anchors.fill: parent
        visible:inputPasswdEnable
        enabled: inputPasswdEnable
        MouseArea
        {
            anchors.fill: parent
            onClicked: {  close()  }
        }
        Rectangle
        {
            width: parent.width*3/5
            height: parent.height*2/5
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            radius: 10
            MouseArea{ anchors.fill: parent }
            Image
            {
                anchors.fill: parent
                source: "../setting/allControl_off.png"
            }
            Column
            {
                anchors.fill: parent
                LuxDomoLabel
                {
                    width: parent.width
                    height: parent.height/4-1
                    textName:pressSsid
                    textSize: 20
                }
                Rectangle
                {
                    width: parent.width
                    height: 1
                    color: "#bfbfbf"
                }
                Item
                {
                    width: parent.width
                    height: parent.height/3
                    Image
                    {
                        width: parent.width*3/5
                        height: parent.height/2
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: parent.width/10
                        source: "../setting/version_number.png"
                    }
                    LuxDomoLabel1
                    {
                        width: parent.width*2/3
                        height: parent.height/2
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: parent.width/10
                        textColor:  pressPsk==""?"red": "#151412"
                        textName: pressPsk==""?"输入密码":pressPsk
                        textSize: 16
                        MouseArea
                        {
                            anchors.fill: parent
                            onClicked: { pLuxDomoWLANSettingItem.editInformation("psk",pressPsk) }
                        }
                    }
                }
                Item
                {
                    width: parent.width
                    height: parent.height*5/12
                    LuxDomoButton
                    {
                        width: parent.width/3
                        height: parent.height/2
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: parent.width/9
                        name: "取消"
                        onClicked: { close()  }
                    }
                    LuxDomoButton
                    {
                        width: parent.width/3
                        height: parent.height/2
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: parent.width/9
                        name: "连接"
                        onClicked:
                        {
                            pLuxDomoWLANSettingItem.linkWLAN(false)
                            close()
                         }
                    }
                }
            }
        }
    }

    Item
    {
        anchors.fill: parent
        visible:optionListEnable
        enabled:optionListEnable
        MouseArea
        {
            anchors.fill: parent
            onClicked: {  close()  }
        }
        Rectangle
        {
            width: parent.width/2
            height: parent.height/2
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            radius: 10
            MouseArea{ anchors.fill: parent }
            Image
            {
                anchors.fill: parent
                source: "../setting/allControl_off.png"
            }
            Column
            {
                anchors.fill: parent
                LuxDomoLabel
                {
                    width: parent.width
                    height: parent.height/4
                    textName: pressSsid
                    textSize: 22
                }
                Rectangle
                {
                    width: parent.width
                    height: 1
                    color: "#bfbfbf"
                }
                LuxDomoLabel1
                {
                    width: parent.width
                    height: parent.height/4-1
                    margin: parent.width/10
                    textName:"删除网络"
                    textSize: 18
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            pLuxDomoWLANSettingItem.deleteWLAN()
                            close()
                        }
                    }
                }
                Rectangle
                {
                    width: parent.width
                    height: 1
                    color: "#bfbfbf"
                    visible: pressRange!="on"?false:true
                    enabled:pressRange!="on"?false:true
                }
                LuxDomoLabel1
                {
                    width: parent.width
                    height: parent.height/4-1
                    margin: parent.width/10
                    textName:"连接网络"
                    textSize: 18
                    visible: pressRange!="on"?false:true
                    enabled:pressRange!="on"?false:true
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            if(pressPsk!=""||pressFlags=="[ESS]")
                            {
                                pLuxDomoWLANSettingItem.linkWLAN(false)
                               close()
                            }
                            else
                            {
                                optionEnable=false
                                optionListEnable=false
                                inputPasswdEnable=true
                            }
                        }
                    }
                }
                Rectangle
                {
                    width: parent.width
                    height: 1
                    color: "#bfbfbf"
                    visible: pressRange!="on"?false:true
                    enabled:pressRange!="on"?false:true
                }
                LuxDomoLabel1
                {
                    width: parent.width
                    height: parent.height/4-1
                    margin: parent.width/10
                    textName:"参数设置"
                    textSize: 18
                    visible: pressRange!="on"?false:true
                    enabled:pressRange!="on"?false:true
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            parameterSettingsEnable=true
                            inputPasswdEnable=false
                            optionEnable=false
                            optionListEnable=false
                            pLuxDomoWLANSettingItem.setWifiParameter(pressBssid)
                        }
                    }
                }
            }
        }
    }

    Item
    {
        anchors.fill: parent
        visible:optionEnable
        enabled: optionEnable
        MouseArea
        {
            anchors.fill: parent
            onClicked: {  close()  }
        }
        Rectangle
        {
            width: parent.width/2
            height: parent.height/2
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            radius: 10
            MouseArea{ anchors.fill: parent }
            Image
            {
                anchors.fill: parent
                source: "../setting/allControl_off.png"
            }
            Column
            {
                anchors.fill: parent
                LuxDomoLabel
                {
                    width: parent.width
                    height: parent.height/4
                    textName: ssid
                    textSize: 22
                }
                Rectangle
                {
                    width: parent.width
                    height: 1
                    color: "#bfbfbf"
                }
                LuxDomoLabel1
                {
                    width: parent.width
                    height: parent.height/4-1
                    margin: parent.width/10
                    textName:"断开网路"
                    textSize: 18
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            pLuxDomoWLANSettingItem.breakWLAN()
                            close()
                        }
                    }
                }
                Rectangle
                {
                    width: parent.width
                    height: 1
                    color: "#bfbfbf"
                }
                LuxDomoLabel1
                {
                    width: parent.width
                    height: parent.height/4-1
                    margin: parent.width/10
                    textName: "删除网络"
                    textSize: 18
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                            pLuxDomoWLANSettingItem.seniorDeleteWLAN()
                            close()
                        }
                    }
                }
            }
        }
    }

    Item
    {
        anchors.fill: parent
        visible:parameterSettingsEnable
        enabled:parameterSettingsEnable
        MouseArea
        {
            anchors.fill: parent
            onClicked: {  close()  }
        }
        Rectangle
        {
            width: parent.width*4/5
            height: parent.height*4/5
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            radius: 10
            MouseArea{ anchors.fill: parent }
            Image
            {
                anchors.fill: parent
                source: "../setting/allControl_off.png"
            }
            Column
            {
                anchors.fill: parent
                LuxDomoLabel
                {
                    width: parent.width
                    height: parent.height/6-1
                    textName: pressSsid
                    textSize: 22
                }
                Rectangle
                {
                    width: parent.width
                    height: 1
                    color: "#bfbfbf"
                }
                Item
                {
                    width: parent.width
                    height: parent.height/6
                    LuxDomoLabel
                    {
                        width: parent.width/4
                        height: parent.height
                        anchors.left: parent.left
                        textName: "Ip网址"
                        textSize: 18
                    }
                    Image
                    {
                        width: parent.width*5/8
                        height: parent.height*3/5
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: parent.width/8
                        source: "../setting/version_number.png"
                    }
                    LuxDomoLabel
                    {
                        width: parent.width*5/8
                        height: parent.height*3/5
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: parent.width/8
                        textColor:  pressIp==""?"red": "#151412"
                        textName: pressIp==""?"必填": pressIp
                        textSize: 18
                    }
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: {  pLuxDomoWLANSettingItem.editInformation("ip",pressIp) }
                    }
                }
                Item
                {
                    width: parent.width
                    height: parent.height/6
                    LuxDomoLabel
                    {
                        width: parent.width/4
                        height: parent.height
                        anchors.left: parent.left
                        textName: "子网掩码"
                        textSize: 18
                    }
                    Image
                    {
                        width: parent.width*5/8
                        height: parent.height*3/5
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: parent.width/8
                        source: "../setting/version_number.png"
                    }
                    LuxDomoLabel
                    {
                        width: parent.width*5/8
                        height: parent.height*3/5
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: parent.width/8
                        textColor:  pressNetmask==""?"red": "#151412"
                        textName: pressNetmask==""?"必填": pressNetmask
                        textSize: 18
                    }
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: {  pLuxDomoWLANSettingItem.editInformation("netmask",pressNetmask) }
                    }
                }
                Item
                {
                    width: parent.width
                    height: parent.height/6
                    LuxDomoLabel
                    {
                        width: parent.width/4
                        height: parent.height
                        anchors.left: parent.left
                        textName: "网关"
                        textSize: 18
                    }
                    Image
                    {
                        width: parent.width*5/8
                        height: parent.height*3/5
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: parent.width/8
                        source: "../setting/version_number.png"
                    }
                    LuxDomoLabel
                    {
                        width: parent.width*5/8
                        height: parent.height*3/5
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: parent.width/8
                        textColor:  pressGateway==""?"red": "#151412"
                        textName: pressGateway==""?"必填": pressGateway
                        textSize: 18
                    }
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: {  pLuxDomoWLANSettingItem.editInformation("gateway",pressGateway) }
                    }
                }
                Item
                {
                    width: parent.width
                    height: parent.height/6
                    LuxDomoLabel
                    {
                        width: parent.width/4
                        height: parent.height
                        anchors.left: parent.left
                        textName: "DNS服务"
                        textSize: 18
                    }
                    Image
                    {
                        width: parent.width*5/8
                        height: parent.height*3/5
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: parent.width/8
                        source: "../setting/version_number.png"
                    }
                    LuxDomoLabel
                    {
                        width: parent.width*5/8
                        height: parent.height*3/5
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: parent.width/8
                        textName: pressDns==""?"可选": pressDns
                        textSize:18
                    }
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked: {  pLuxDomoWLANSettingItem.editInformation("dns",pressDns) }
                    }
                }
                Item
                {
                    width: parent.width
                    height: parent.height/6
                    LuxDomoButton
                    {
                        width: parent.width/2
                        height: parent.height/2
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        name: "连接"
                        onClicked:
                        {
                            if(pressIp!=""&&pressGateway!=""&&pressNetmask!="")
                            {
                                pLuxDomoWLANSettingItem.linkWLAN(true)
                                close()
                            }
                        }
                    }
                }
            }
        }
    }
}
