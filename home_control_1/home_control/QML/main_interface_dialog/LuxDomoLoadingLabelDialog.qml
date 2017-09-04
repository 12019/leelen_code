import  QtQuick 1.0
Rectangle
{
    id:loading
    objectName: "loading"
    width: 800
    height: 480
    color: "#00000000"

    MouseArea{  anchors.fill: parent  }

    Image
    {
        id: loadingImage
        width: 40
        height: 40
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        source: "../main_interface/loading.png"
        NumberAnimation on rotation
        {
               from: 0; to: 360; running: loading.visible == true; loops: Animation.Infinite; duration: 500
        }
    }
}

