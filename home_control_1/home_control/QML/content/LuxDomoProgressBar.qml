import QtQuick 1.0
Rectangle
{
    id:progressBar
    objectName: "progressBar"
    width: 300
    height: 20
    color: "#00000000"

    signal    positionchanged
    property    string       progressBarImage1: ""
    property    string       progressBarImage2 : ""
    property    string       progressButton:""
    property    int            progressValue:100

    function    changData()
    {
        movingButton.x=progressBar.progressValue/100*(progressBar.width-movingButton.width)
        image1.width=progressBar.progressValue/100*(progressBar.width-movingButton.width)
    }

    Image
    {
        width: parent.width
        height: parent.height/4
        anchors.verticalCenter: parent.verticalCenter
        source: progressBar.progressBarImage2
    }
    Image
    {
        id:image1
        height: parent.height/4
        anchors.verticalCenter: parent.verticalCenter
        width:progressBar.progressValue/100*(progressBar.width-movingButton.width)
        source:progressBar.progressBarImage1
    }
    Item
    {
        id:movingButton
        width: parent.height
        height:parent.height
        x:progressBar.progressValue/100*(progressBar.width-movingButton.width)
        anchors.verticalCenter: parent.verticalCenter
        Image
        {
            anchors.fill: parent
            source: progressBar.progressButton
        }
        MouseArea
        {
            drag.target:movingButton
            drag.axis: Drag.XAxis
            anchors.fill: parent
            onPositionChanged:
            {
                if(movingButton.x<0)
                {
                    movingButton.x=0
                }
                else   if(movingButton.x>progressBar.width-movingButton.width)
                {
                    movingButton.x=progressBar.width-movingButton.width
                }
                progressValue=movingButton.x/(progressBar.width-movingButton.width)*100
                progressBar.positionchanged ()
                console.log(progressValue,movingButton.x)
            }
        }
    }
}
