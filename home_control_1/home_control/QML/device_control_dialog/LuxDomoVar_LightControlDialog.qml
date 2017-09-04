import  QtQuick 1.0
Rectangle
{
       id:var_lightControl
       objectName: "var_lightControl"
       width: 800
       height: 480
       color: "#f1dede"

       function  init(roomUid,deviceUid){  pLuxDomoVar_LightControlItem.init(roomUid,deviceUid)  }
       MouseArea  {   anchors.fill: parent     }
}

