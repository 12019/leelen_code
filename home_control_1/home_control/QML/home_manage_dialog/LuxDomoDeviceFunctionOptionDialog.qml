import    QtQuick 1.0
import    LuxDomoDeviceFunctionOption  1.0
import    "../content"

Rectangle
{
    id:functionOption
    objectName: "functionOption"
    width: 700
    height: 450
    color:"#af000000"

    property    string    deivceUid: ""
    property    string    deivceType: ""
    property    string    deviceName: ""
    property    string    deleteButtonName: ""
    property    string    editButtonName: ""
    property    string    learnButtonName: ""
    property    bool      isShowlearnButton: false
    property    bool      isShowhint: false

    LuxDomoDeviceFunctionOptionItem
    {
          id:pLuxDomoDeviceFunctionOptionItem
          objectName: "pLuxDomoDeviceFunctionOptionItem"
    }

    MouseArea
    {
          anchors.fill: parent
          onClicked: {  pLuxDomoDeviceFunctionOptionItem.close()  }
    }

    Item
    {
        width: parent.width/3
        height: parent.height*4/8
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        enabled: isShowhint==false?true:false
        visible: isShowhint==false?true:false
        Image
        {
            anchors.fill: parent
            source: "../home_manage/functionOption_bottom.png"
        }
        Column
        {
            anchors.fill: parent
            Item
            {
                width: parent.width
                height: parent.height/4
                Rectangle
                {
                   width: parent.width*9/10
                   height: 1
                   anchors.horizontalCenter: parent.horizontalCenter
                   anchors.bottom: parent.bottom
                   color: "#bfbfbf"
                }
                LuxDomoLabel
                {
                    anchors.fill: parent
                    textName: "功能选项"
                    textSize: 27
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/4
                LuxDomoButton
                {
                    width: parent.width*3/5
                    height: parent.height/2
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
//                    image:"../option_off.png"
//                    imagepressed: "../option_off.png"
                    name:functionOption.deleteButtonName
                    fontsize: 20
                    onClicked: {  isShowhint=true  }
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/4
                LuxDomoButton
                {
                    width: parent.width*3/5
                    height: parent.height/2
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
//                    image:"../option_off.png"
//                    imagepressed: "../option_off.png"
                    name:functionOption.editButtonName
                    fontsize: 20
                    onClicked: {  pLuxDomoDeviceFunctionOptionItem.showDeviceEdit()   }
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/4
                LuxDomoButton
                {
                    width: parent.width*3/5
                    height: parent.height/2
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
//                    image:"../option_off.png"
//                    imagepressed: "../option_off.png"
                    name:functionOption.learnButtonName
                     visible: functionOption.isShowlearnButton
                    fontsize: 20
                    onClicked: {  pLuxDomoDeviceFunctionOptionItem.showInfraredLearn()   }
                }
            }
        }
    }

    LuxDomoHintHurdle
    {
        width: parent.width*2/5
        height: parent.height/3
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        enabled: isShowhint
        visible: isShowhint
        onCancelClicked: {    pLuxDomoDeviceFunctionOptionItem.close()  }
        onSureClicked: {      pLuxDomoDeviceFunctionOptionItem.deleteDevice() }
    }
}
