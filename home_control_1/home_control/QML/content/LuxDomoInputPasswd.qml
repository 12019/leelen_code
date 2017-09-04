import QtQuick 1.0

Rectangle
{
    id:inputPasswd
    objectName: "inputPasswd"
    width: 896
    height: 600

    signal  checkPasswd
    property string   passwd: ""
    property string   passwd1: ""

    MouseArea{  anchors.fill: parent  }

    Image
    {
        anchors.fill: parent
        source: "../menu/passwd_background.png"
    }

    Column
    {
        width: parent.width*3/5
        height: parent.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        Item
        {
            width: parent.width
            height: parent.height/5
            Image
            {
                width: parent.width*4/5
                height: parent.height/2
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                source: "../menu/input_passwd.png"
            }
            LuxDomoLabel
            {
                width: parent.width
                height: parent.height/2
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                textName: passwd==""?"请输入正确的六位密码":passwd1
            }
        }
        Item
        {
            width: parent.width
            height: parent.height/5
            Row
            {
                width: parent.width
                height: parent.height*4/5
                Item
                {
                    width: parent.width/3
                    height: parent.height
                    LuxDomoButton
                    {
                       width: parent.height
                       height: parent.height
                       anchors.verticalCenter: parent.verticalCenter
                       anchors.horizontalCenter: parent.horizontalCenter
                       image:"../menu/1_up.png"
                       imagepressed:"../menu/1_down.png"
                       onClicked:
                       {
                           if(passwd.length<6)
                           {
                               passwd+="1"
                               passwd1+="●"
                           }
                       }
                    }
                }
                Item
                {
                    width: parent.width/3
                    height: parent.height
                    LuxDomoButton
                    {
                       width: parent.height
                       height: parent.height
                       anchors.verticalCenter: parent.verticalCenter
                       anchors.horizontalCenter: parent.horizontalCenter
                       image:"../menu/2_up.png"
                       imagepressed:"../menu/2_down.png"
                       onClicked:
                       {
                           if(passwd.length<6)
                           {
                               passwd+="2"
                               passwd1+="●"
                           }
                       }
                    }
                }
                Item
                {
                    width: parent.width/3
                    height: parent.height
                    LuxDomoButton
                    {
                       width: parent.height
                       height: parent.height
                       anchors.verticalCenter: parent.verticalCenter
                       anchors.horizontalCenter: parent.horizontalCenter
                       image:"../menu/3_up.png"
                       imagepressed: "../menu/3_down.png"
                       onClicked:
                       {
                           if(passwd.length<6)
                           {
                               passwd+="3"
                               passwd1+="●"
                           }
                       }
                    }
                }
            }
        }
        Item
        {
            width: parent.width
            height: parent.height/5
            Row
            {
                width: parent.width
                height: parent.height*4/5
                Item
                {
                    width: parent.width/3
                    height: parent.height
                    LuxDomoButton
                    {
                       width: parent.height
                       height: parent.height
                       anchors.verticalCenter: parent.verticalCenter
                       anchors.horizontalCenter: parent.horizontalCenter
                       image:"../menu/4_up.png"
                       imagepressed:"../menu/4_down.png"
                       onClicked:
                       {
                           if(passwd.length<6)
                           {
                               passwd+="4"
                               passwd1+="●"
                           }
                       }
                    }
                }
                Item
                {
                    width: parent.width/3
                    height: parent.height
                    LuxDomoButton
                    {
                       width: parent.height
                       height: parent.height
                       anchors.verticalCenter: parent.verticalCenter
                       anchors.horizontalCenter: parent.horizontalCenter
                       image:"../menu/5_up.png"
                       imagepressed:"../menu/5_down.png"
                       onClicked:
                       {
                           if(passwd.length<6)
                           {
                               passwd+="5"
                               passwd1+="●"
                           }
                       }
                    }
                }
                Item
                {
                    width: parent.width/3
                    height: parent.height
                    LuxDomoButton
                    {
                       width: parent.height
                       height: parent.height
                       anchors.verticalCenter: parent.verticalCenter
                       anchors.horizontalCenter: parent.horizontalCenter
                       image:"../menu/6_up.png"
                       imagepressed: "../menu/6_down.png"
                       onClicked:
                       {
                           if(passwd.length<6)
                           {
                               passwd+="6"
                               passwd1+="●"
                           }
                       }
                    }
                }
            }
        }
        Item
        {
            width: parent.width
            height: parent.height/5
            Row
            {
                width: parent.width
                height: parent.height*4/5
                Item
                {
                    width: parent.width/3
                    height: parent.height
                    LuxDomoButton
                    {
                       width: parent.height
                       height: parent.height
                       anchors.verticalCenter: parent.verticalCenter
                       anchors.horizontalCenter: parent.horizontalCenter
                       image:"../menu/7_up.png"
                       imagepressed:"../menu/7_down.png"
                       onClicked:
                       {
                           if(passwd.length<6)
                           {
                               passwd+="7"
                               passwd1+="●"
                           }
                       }
                    }
                }
                Item
                {
                    width: parent.width/3
                    height: parent.height
                    LuxDomoButton
                    {
                       width: parent.height
                       height: parent.height
                       anchors.verticalCenter: parent.verticalCenter
                       anchors.horizontalCenter: parent.horizontalCenter
                       image:"../menu/8_up.png"
                       imagepressed:"../menu/8_down.png"
                       onClicked:
                       {
                           if(passwd.length<6)
                           {
                               passwd+="8"
                               passwd1+="●"
                           }
                       }
                    }
                }
                Item
                {
                    width: parent.width/3
                    height: parent.height
                    LuxDomoButton
                    {
                       width: parent.height
                       height: parent.height
                       anchors.verticalCenter: parent.verticalCenter
                       anchors.horizontalCenter: parent.horizontalCenter
                       image:"../menu/9_up.png"
                       imagepressed: "../menu/9_down.png"
                       onClicked:
                       {
                           if(passwd.length<6)
                           {
                               passwd+="9"
                               passwd1+="●"
                           }
                       }
                    }
                }
            }
        }
        Item
        {
            width: parent.width
            height: parent.height/5
            Row
            {
                width: parent.width
                height: parent.height*4/5
                Item
                {
                    width: parent.width/3
                    height: parent.height
                    LuxDomoButton
                    {
                       width: parent.height
                       height: parent.height
                       anchors.verticalCenter: parent.verticalCenter
                       anchors.horizontalCenter: parent.horizontalCenter
                       image:"../menu/10_up.png"
                       imagepressed:"../menu/10_down.png"
                       onClicked:
                       {
                           if(passwd.length>0)
                           {
                               passwd=passwd.substring(0,passwd.length-1)
                               passwd1=passwd1.substring(0,passwd1.length-1)
                           }
                       }
                    }
                }
                Item
                {
                    width: parent.width/3
                    height: parent.height
                    LuxDomoButton
                    {
                       width: parent.height
                       height: parent.height
                       anchors.verticalCenter: parent.verticalCenter
                       anchors.horizontalCenter: parent.horizontalCenter
                       image:"../menu/0_up.png"
                       imagepressed:"../menu/0_down.png"
                       onClicked:
                       {
                           if(passwd.length<6)
                           {
                               passwd+="0"
                               passwd1+="●"
                           }
                       }
                    }
                }
                Item
                {
                    width: parent.width/3
                    height: parent.height
                    LuxDomoButton
                    {
                       width: parent.height
                       height: parent.height
                       anchors.verticalCenter: parent.verticalCenter
                       anchors.horizontalCenter: parent.horizontalCenter
                       image:"../menu/11_up.png"
                       imagepressed: "../menu/11_down.png"
                       onClicked: {   inputPasswd.checkPasswd()   }
                    }
                }
            }
        }
    }
}
