import   QtQuick 1.0
import   LuxDomoTvLearn  1.0
import   "../content"
Rectangle
{
    id:tvLearn
    objectName: "tvLearn"
    width: 800
    height: 480
    color:"#efeff4"

    property    int         pressIndex: -1
    property    bool      isShowIrHint: false

    function addModel(code,operateName,isLearned)
    {
        tvModel.append (
                    {
                        "code":code,
                        "operateName":operateName,
                        "isLearned":isLearned
                    }  )
    }

    function  clearModel()   {      tvModel.clear()     }
    function  init(deviceUid){    pLuxDomoTvLearnItem.init(deviceUid)  }
    function     changModel(propertyName,propertyValue){  tvModel.setProperty( pressIndex,propertyName,propertyValue) }
    function     learnCode(){  pLuxDomoTvLearnItem.learnCode()  }

    LuxDomoTvLearnItem
    {
          id:pLuxDomoTvLearnItem
          objectName: "pLuxDomoTvLearnItem"
    }

    MouseArea {   anchors.fill: parent   }

    Rectangle
    {
        id:titleBar
        objectName: titleBar
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
            }
            Item
            {
                width: parent.width/2
                height: parent.height
                LuxDomoLabel
                {
                    anchors.fill: parent
                    textName: "电视红外学习"
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
                    name:"保存"
                    textcolor: "#24c3be"
                    fontsize: 22
                    onClicked:  {  pLuxDomoTvLearnItem.close()   }
                }
            }
        }
    }

    ListModel
    {
             id: tvModel
             ListElement
             {
                 code:1
                 operateName:"灯1"
                 isLearned: false
             }
    }

    Component
     {
         id:tvDelegate
         Rectangle
         {
             id:tvOperate
             objectName: "musicOperate"
             width: tvList.width
             height: tvList.height/6
             color: pressIndex==index?"#24c3be":"white"
             Row
             {
                 anchors.fill: parent
                 LuxDomoLabel
                 {
                        width:parent.width/3
                        height: parent.height
                        textName:operateName
                 }
                 Item
                 {
                     width:parent.width/3
                     height: parent.height
                 }
                 Item
                 {
                     width:parent.width/3
                     height: parent.height
                     Image
                     {
                          width: parent.width/2
                          height: parent.height/2
                          anchors.verticalCenter: parent.verticalCenter
                          source: isLearned?"../home_manage/learn_on.png":"../home_manage/learn_off.png"
                     }
                     LuxDomoLabel
                     {
                         width: parent.width/2
                         height: parent.height/2
                         anchors.verticalCenter: parent.verticalCenter
                         textName: isLearned?"已学习":"未学习"
                     }
                   }
                }
             MouseArea
             {
                 anchors.fill: parent
                 onClicked:
                 {
                     tvLearn.pressIndex=index
                     isShowIrHint=true
                     pLuxDomoTvLearnItem.learnClicked(code)
                  }
            }
          }
     }

    ListView
    {
        id: tvList
        width: parent.width
        height:parent.height*7/8
        y:parent.height/8
        clip: true
        model: tvModel
        delegate: tvDelegate
     }

    LuxDomoHintIrLearn
    {
        width: parent.width/2
        height: parent.height*2/5
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        enabled: isShowIrHint
        visible: isShowIrHint
        onClicked1:
        {
            pLuxDomoTvLearnItem.cancelLearn()
            isShowIrHint=false
        }
    }
}

