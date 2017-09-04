import  QtQuick 1.0
import  "../content"
import  LuxDomoRisco  1.0
Rectangle
{
    id:risco
    objectName: "risco"
    width: 896
    height: 600
    color:"#efeff4"

   property   string   condition: ""

   MouseArea{ anchors.fill: parent  }

   LuxDomoRiscoItem
   {
       id:pLuxDomoRiscoItem
       objectName: "pLuxDomoRiscoItem"
   }

    Column
    {
        anchors.fill: parent
        Rectangle
        {
            width: parent.width
            height: parent.height/8
            color:"#f6f6f7"
            LuxDomoLabel
            {
                anchors.fill: parent
                textName: "Risco安防"
                textSize: 30
            }
        }
        Item
        {
            width: parent.width
            height: parent.height/10
            Row
            {
                anchors.fill: parent
                Item
                {
                     width: parent.width/4
                     height: parent.height
                     Image
                     {
                         anchors.fill: parent
                         source: condition=="security"?"../security/security_down.png":"../security/security_up.png"
                     }
                     Row
                     {
                         anchors.fill: parent
                         Item
                         {
                               width: parent.width/2
                               height: parent.height
                               Image
                               {
                                   width: parent.height*2/3
                                   height: parent.height*2/3
                                   anchors.verticalCenter: parent.verticalCenter
                                   anchors.right: parent.right
                                   source:"../security/security_security.png"
                               }
                         }
                         Item
                         {
                             width: parent.width/2
                             height: parent.height
                             LuxDomoLabel1
                             {
                                 anchors.fill: parent
                                 textName: "安保"
                                 textSize: 25
                                 margin: 10
                             }
                         }
                     }
                     MouseArea
                     {
                         anchors.fill: parent
                         onClicked:
                         {
                             condition="security"
                             pLuxDomoRiscoItem.chooseFunction("security")
                         }
                     }
                }
                Item
                {
                    width: parent.width/4
                    height: parent.height
                    Image
                    {
                        anchors.fill: parent
                        source: condition=="finder"?"../security/security_down.png":"../security/security_up.png"
                    }
                    Row
                    {
                        anchors.fill: parent
                        Item
                        {
                              width: parent.width*2/5
                              height: parent.height
                              Image
                              {
                                  width: parent.height*2/3
                                  height: parent.height*2/3
                                  anchors.verticalCenter: parent.verticalCenter
                                  anchors.right: parent.right
                                  source: "../security/security_finder.png"
                              }
                        }
                        Item
                        {
                            width: parent.width*3/5
                            height: parent.height
                            LuxDomoLabel1
                            {
                                anchors.fill: parent
                                textName: "探测器"
                                textSize: 25
                                margin: 10
                            }
                        }
                    }
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                             condition="finder"
                             pLuxDomoRiscoItem.chooseFunction("finder")
                        }
                    }
                }
                Item
                {
                    width: parent.width/4
                    height: parent.height
                    Image
                    {
                        anchors.fill: parent
                        source: condition=="control"?"../security/security_down.png":"../security/security_up.png"
                    }
                    Row
                    {
                        anchors.fill: parent
                        Item
                        {
                              width: parent.width/3
                              height: parent.height
                              Image
                              {
                                  width: parent.height*2/3
                                  height: parent.height*2/3
                                  anchors.verticalCenter: parent.verticalCenter
                                  anchors.right: parent.right
                                  source: "../security/security_control.png"
                              }
                        }
                        Item
                        {
                            width: parent.width*2/3
                            height: parent.height
                            LuxDomoLabel1
                            {
                                anchors.fill: parent
                                textName: "控制输出"
                                textSize: 25
                                margin: 10
                            }
                        }
                    }
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                             condition="control"
                             pLuxDomoRiscoItem.chooseFunction("control")
                        }
                    }
                }
                Item
                {
                    width: parent.width/4
                    height: parent.height
                    Image
                    {
                        anchors.fill: parent
                        source: condition=="message"?"../security/security_down.png":"../security/security_up.png"
                    }
                    Row
                    {
                        anchors.fill: parent
                        Item
                        {
                              width: parent.width/3
                              height: parent.height
                              Image
                              {
                                  width: parent.height*2/3
                                  height: parent.height*2/3
                                  anchors.verticalCenter: parent.verticalCenter
                                  anchors.right: parent.right
                                  source: "../security/security_message.png"
                              }
                        }
                        Item
                        {
                            width: parent.width*2/3
                            height: parent.height
                            LuxDomoLabel1
                            {
                                anchors.fill: parent
                                textName: "事件记录"
                                textSize: 25
                                margin: 10
                            }
                        }
                    }
                    MouseArea
                    {
                        anchors.fill: parent
                        onClicked:
                        {
                             condition="message"
                             pLuxDomoRiscoItem.chooseFunction("message")
                        }
                    }
                }
            }
        }
    }
}

