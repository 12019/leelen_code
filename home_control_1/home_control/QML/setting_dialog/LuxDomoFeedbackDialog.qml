import   QtQuick 1.0
import   "../content"
import   LuxDomoFeedbackSetting  1.0
Rectangle
{
    id:feedbackSetting
    objectName: "feedbackSetting"
    width: 450
    height: 480
    color:"#efeff4"

    property  string    message: ""

    function    clearModel()      {    model.clear()   }
    function    addModel(listData,isOption){  model.append({ "listData":listData,"isOption":isOption })  }
    function    changModel(index,propertyName,propertyValue){  model.setProperty( index,propertyName,propertyValue) }
    function    setInformation(data){  pLuxDomoFeedbackSettingItem.setInformation(data)  }

    MouseArea{  anchors.fill: parent  }

    Component.onCompleted: {  pLuxDomoFeedbackSettingItem.init()    }

    LuxDomoFeedbackSettingItem
    {
        id:pLuxDomoFeedbackSettingItem
        objectName: "pLuxDomoFeedbackSettingItem"
    }

    Rectangle
    {
        width: parent.width
        height: parent.height/8
        anchors.top: parent.top
        color:"#f6f6f7"
        Rectangle
        {
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
            color: "#bfbfbf"
        }
        Row
        {
            anchors.fill: parent
            Item
            {
                width: parent.width/4
                height: parent.height
                LuxDomoButton
                {
                    width: parent.width/2
                    height: parent.height*2/3
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    name:"取消"
                    textcolor: "#24c3be"
                    fontsize: 22
                    showImage: false
                    onClicked: {   pLuxDomoFeedbackSettingItem.cancelFeedback() }
                }
            }
            Item
            {
                width: parent.width/2
                height: parent.height
                LuxDomoLabel
                {
                    anchors.fill: parent
                    textName: "用户反馈"
                    textSize: 27
                }
            }
            Item
            {
                width: parent.width/4
                height: parent.height
                LuxDomoButton
                {
                    width: parent.width/2
                    height: parent.height*2/3
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    name:"发送"
                    textcolor: "#24c3be"
                    fontsize: 22
                    showImage: false
                    onClicked:  {  pLuxDomoFeedbackSettingItem.sendFeedback() }
                }
            }
        }
    }

    LuxDomoLabel1
    {
        width: parent.width*9/10
        height: parent.height/8
        x:parent.width/20
        y:parent.height/8
        textName: "出现异常的类型(可附加文字描述)"
        textSize: 22
        textColor: "#676767"
    }

    ListModel
    {
        id: model
        ListElement
        {
               listData:"NULL"
               isOption:false
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
             Item
             {
                 width: parent.width*19/20
                 height: parent.height
                 anchors.right:parent.right
                 Rectangle
                 {
                     width: parent.width
                     height: 1
                     anchors.right:parent.right
                     anchors.bottom: parent.bottom
                     color: "#bfbfbf"
                 }
                 Row
                 {
                     anchors.fill: parent
                     Item
                     {
                         width: parent.width/6
                         height: parent.height
                         Image
                         {
                             width: parent.height*3/5
                             height: parent.height*3/5
                             anchors.verticalCenter: parent.verticalCenter
                             source: isOption?"../setting/enable1.png":"../setting/disenable1.png"
                         }
                     }
                     Item
                     {
                         width: parent.width/6
                         height: parent.height
                         LuxDomoLabel1
                         {
                             anchors.fill: parent
                             textName: listData
                             textSize: 20
                         }
                     }
                 }
                 MouseArea
                 {
                     anchors.fill: parent
                     onClicked: {  pLuxDomoFeedbackSettingItem.optionType(index,listData) }
                 }
             }
         }
    }

    ListView
    {
        id: list
        width: parent.width*9/10
        height: parent.height/2
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        clip: true
        model: model
        delegate: delegate
    }

    Rectangle
    {
       width: parent.width*9/10
       height: parent.height*3/16
       anchors.horizontalCenter: parent.horizontalCenter
       anchors.bottom: parent.bottom
       LuxDomoLabel2
       {
           anchors.fill: parent
           textName:message==""? "请输入描述信息":message
           textColor:message==""? "#676767":"#151412"
           textSize: 20
       }
       MouseArea
       {
           anchors.fill: parent
           onClicked: {     pLuxDomoFeedbackSettingItem.editMessage()   }
       }
    }
}

