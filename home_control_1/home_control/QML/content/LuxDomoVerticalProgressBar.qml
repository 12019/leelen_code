import QtQuick 1.0
Rectangle
{
    id:progressBar
    objectName: "progressBar"
    width: 20
    height: 250
    color: "#00000000"

    signal    positionchanged
    property    string       topImage : ""
    property    string       bottomImage: ""
    property    string       progressBarImage1: ""
    property    string       progressBarImage2 : ""
    property    string       progressButton:""
    property    int            progressValue:20

    function   changData()
    {
         movingButton.y=(100-progressValue)/100*(progress.height*4/5)+(progress.height/10-movingButton.height/2)
    }

    Column
    {
       anchors.fill: parent
       Item
       {
               id:top
               width: parent.width
               height: parent.height/10
               anchors.horizontalCenter: parent.horizontalCenter
               LuxDomoButton
               {
                   width: parent.width
                   height: parent.width
                   anchors.verticalCenter: parent.verticalCenter
                   anchors.horizontalCenter: parent.horizontalCenter
                   image: progressBar.topImage
                   imagepressed:progressBar.topImage
               }
       }
       Item
       {
           id:progress
           width: parent.width
           height: parent.height*4/5
           Image
           {
               width: parent.width/4
               height: parent.height*4/5
               anchors.horizontalCenter: parent.horizontalCenter
               anchors.verticalCenter: parent.verticalCenter
               source: progressBar.progressBarImage2
           }
           Image
           {
               width: parent.width/4
               anchors.horizontalCenter: parent.horizontalCenter
               y:parent.height/10
               height: (100-progressValue)/100*(progress.height*4/5)
               source:progressBar.progressBarImage1
           }
           Item
           {
                    id:movingButton
                    width: parent.width*2/3
                    height:parent.width*2/3
                    y:225
                    anchors.horizontalCenter: parent.horizontalCenter
                    onYChanged:
                    {  console.log(movingButton.y,"HHHHHHHHHHHHHHHHHHHHHHHHHHH")
                        if(movingButton.y<parent.height/10-movingButton.height/2)
                        {
                            movingButton.y=parent.height/10-movingButton.height/2
                        }
                        else    if(movingButton.y>parent.height*9/10-movingButton.height/2)
                        {
                            movingButton.y=parent.height*9/10-movingButton.height/2
                        }
                    }
                    Image
                    {
                        anchors.fill: parent
                        source: progressBar.progressButton
                    }
                    MouseArea
                    {
                            drag.target:movingButton
                            drag.axis: Drag.YAxis
                            anchors.fill: parent
                            onPositionChanged:
                            {
                                var  pointY=(movingButton.y-(progress.height/10-movingButton.height/2))*100
                                progressBar.progressValue=pointY/(progress.height*4/5);
                                progressBar.progressValue=100-progressBar.progressValue
                                progressBar.positionchanged ()
                            }
                    }
           }
       }
       Item
       {
           id:bottom
           width: parent.width
           height: parent.height/10
           anchors.horizontalCenter: parent.horizontalCenter
           LuxDomoButton
           {
               width: parent.height
               height: parent.height
               anchors.verticalCenter: parent.verticalCenter
               anchors.horizontalCenter: parent.horizontalCenter
               image:progressBar.bottomImage
               imagepressed:progressBar.bottomImage
           }
       }
    }
}

