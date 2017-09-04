import   QtQuick 1.0
import   LuxDomoMusicControl  1.0
import   "../content"
Rectangle
{
    id:musicControl
    objectName: "musicControl"
    width: 800
    height: 480
    color: "#00000000"

    property   string     deviceUid: ""
    property   bool      musicCondition: false
    property   string     musicMode: " "
    property   bool      muiscIsRunning: false
    property   int          musicVolume: 0
    property   string     musicName: ""
    property   string     songName: ""
    property   int          musicGallery: 0
    property   bool      musicMute: false

    function   init(roomUid,deviceUid){   pLuxDomoMusicControlItem.init(roomUid,deviceUid)  }
    function   findMusicInformation(){   pLuxDomoMusicControlItem.findMusicInformation()  }

    onMusicVolumeChanged:
    {
        volume.progressValue=musicVolume
        volume.changData()
    }

    LuxDomoMusicControlItem
    {
        id:pLuxDomoMusicControlItem
        objectName: "pLuxDomoMusicControlItem"
    }

    MouseArea
    {
        anchors.fill: parent
        onClicked: { pLuxDomoMusicControlItem.close() }
    }

    Item
    {
        width: parent.width*3/5
        height: parent.height*13/16
        anchors.horizontalCenter: parent.horizontalCenter
        y:parent.height*5/32
        MouseArea{  anchors.fill: parent  }
        Image
        {
            anchors.fill: parent
            source: "../device_control/music_background.png"
        }
        Column
        {
            anchors.fill: parent
            Item
            {
                width: parent.width
                height: parent.height/8
                LuxDomoLabel6
                {
                    anchors.fill: parent
                    textName: musicName+"控制"
                    textSize: 22
                }
            }
            Item
            {
                width: parent.width
                height: parent.height*17/24
                Row
                {
                    anchors.fill: parent
                    Item
                    {
                        width: parent.width/4
                        height: parent.height
                        Column
                        {
                            width: parent.width
                            height: parent.height*9/10
                            Item
                            {
                                width: parent.width
                                height: parent.height/3
                                LuxDomoButton
                                {
                                    width: parent.height/2
                                    height: parent.height/2
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    image:"../device_control/power_off.png"
                                    imagepressed: "../device_control/power_on.png"
                                    onClicked:
                                    {
                                        pLuxDomoMusicControlItem.musicPowerClicked()
                                    }
                                }
                                LuxDomoLabel
                                {
                                    width: parent.width
                                    height: parent.height/4
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: parent.height/4
                                    textName:"开关"
                                }
                            }
                            Item
                            {
                                width: parent.width
                                height: parent.height/3
                                LuxDomoButton
                                {
                                    width: parent.height/2
                                    height: parent.height/2
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    image:"../device_control/music_gallery_off.png"
                                    imagepressed: "../device_control/music_gallery_on.png"
                                    onClicked:
                                    {
                                         pLuxDomoMusicControlItem.musicChannelClicked()
                                    }
                                }
                                LuxDomoLabel
                                {
                                    width: parent.width
                                    height: parent.height/4
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: parent.height/4
                                    textName: "通道"
                                }
                            }
                            Item
                            {
                                width: parent.width
                                height: parent.height/3
                                LuxDomoButton
                                {
                                    width: parent.height/2
                                    height: parent.height/2
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    enabled: musicCondition
                                    image:"../device_control/music_mode_off.png"
                                    imagepressed: "../device_control/music_mode_on.png"
                                    onClicked:
                                    {
                                        pLuxDomoMusicControlItem.musicModeClicked()
                                    }
                                }
                                LuxDomoLabel
                                {
                                    width: parent.width
                                    height: parent.height/4
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: parent.height/4
                                    textName: "模式"
                                }
                            }
                        }
                    }
                    Item
                    {
                        width: parent.width/2
                        height: parent.height
                        Image
                        {
                            id:deviceImage
                            width: parent.width*7/8
                            height: parent.width*7/8
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            source: musicCondition?"../device_control/music_open.png":"../device_control/music_close.png"
                        }
                        Item
                        {
                            width: deviceImage.width
                            height: deviceImage.height/5
                            anchors.top: deviceImage.top
                            anchors.topMargin: deviceImage.height*9/64
                            anchors.horizontalCenter: parent.horizontalCenter 
                            Column
                            {
                                anchors.fill: parent
                               Item
                               {
                                   width: parent.width
                                   height: parent.height/2
                                   LuxDomoLabel
                                   {
                                       anchors.fill: parent
                                       visible: musicCondition
                                       textName: songName==""?"载入中......":songName
                                       textSize: 15
                                   }
                               }
                               Item
                               {
                                   width: parent.width
                                   height: parent.height/2
                                   LuxDomoImageLabel
                                   {
                                       width: parent.width*2/5
                                       height: parent.height
                                       anchors.right: parent.right
                                       anchors.rightMargin: parent.width/2
                                       textName: "通道"+musicGallery
                                       textSize: 15
                                       imageStr: "../device_control/music_gallery.png"
                                   }
                                   LuxDomoImageLabel
                                   {
                                       width: parent.width/3
                                       height: parent.height
                                       visible: musicCondition
                                       x:parent.width/2
                                       textName: musicMode
                                       textSize: 15
                                       imageStr: "../device_control/music_mode.png"
                                   }
                               }
                            }
                        }
                    }
                    Item
                    {
                        width: parent.width/4
                        height: parent.height
                        Column
                        {
                            width: parent.width
                            height: parent.height*9/10
                            Item
                            {
                                width: parent.width
                                height: parent.height/3
                                LuxDomoButton
                                {
                                    width: parent.height/2
                                    height: parent.height/2
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    enabled: musicCondition
                                    image:"../device_control/music_previous_off.png"
                                    imagepressed: "../device_control/music_previous_on.png"
                                    onClicked:
                                    {
                                        pLuxDomoMusicControlItem.musicPreviousClicked()
                                    }
                                }
                                LuxDomoLabel
                                {
                                    width: parent.width
                                    height: parent.height/4
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: parent.height/4
                                    textName: "上一首"
                                }
                            }
                            Item
                            {
                                width: parent.width
                                height: parent.height/3
                                LuxDomoButton
                                {
                                    width: parent.height/2
                                    height: parent.height/2
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    enabled: musicCondition
                                    image: muiscIsRunning?"../device_control/music_stop_off.png":"../device_control/music_run_off.png"
                                    imagepressed: muiscIsRunning?"../device_control/music_stop_on.png":"../device_control/music_run_on.png"
                                    onClicked:
                                    {
                                        if(muiscIsRunning)
                                        {
                                            muiscIsRunning=false
                                            pLuxDomoMusicControlItem.musicPauseClicked("off")
                                        }
                                        else
                                        {
                                            muiscIsRunning=true
                                            pLuxDomoMusicControlItem.musicPauseClicked("on")
                                        }
                                       console.log(muiscIsRunning)
                                    }
                                }
                                LuxDomoLabel
                                {
                                    width: parent.width
                                    height: parent.height/4
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: parent.height/4
                                    textName:muiscIsRunning?"暂停":"播放"
                                }
                            }
                            Item
                            {
                                width: parent.width
                                height: parent.height/3
                                LuxDomoButton
                                {
                                    width: parent.height/2
                                    height: parent.height/2
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    enabled: musicCondition
                                    image:"../device_control/music_next_off.png"
                                    imagepressed: "../device_control/music_next_on.png"
                                    onClicked:
                                    {
                                        pLuxDomoMusicControlItem.musicNextClicked()
                                    }
                                }
                                LuxDomoLabel
                                {
                                    width: parent.width
                                    height: parent.height/4
                                    anchors.bottom: parent.bottom
                                    anchors.bottomMargin: parent.height/4
                                    textName: "下一首"
                                }
                            }
                        }
                    }
                }
            }
            Item
            {
                width: parent.width
                height: parent.height/6
                Row
                {
                    anchors.fill: parent
                    Item
                    {
                        width: parent.width/4
                        height: parent.height
                        LuxDomoButton
                        {
                            width: parent.height/2
                            height: parent.height/2
                            anchors.horizontalCenter: parent.horizontalCenter
                            y:0
                            image:musicMute?"../device_control/music_volume_mute.png":"../device_control/music_volume_small.png"
                            imagepressed: image
                            onClicked:
                            {
                                if(musicMute)
                                {
                                    musicMute=false
                                    pLuxDomoMusicControlItem.musicMuteClicked("off")
                                }
                                else
                                {
                                    musicMute=true
                                    pLuxDomoMusicControlItem.musicMuteClicked("on")
                                }
                            }
                        }
                    }
                    Item
                    {
                        width: parent.width/2
                        height: parent.height
                        Item
                       {
                           id:value
                           width: parent.width
                           height: parent.height/2
                           y:-parent.height*7/12
                           anchors.horizontalCenter: parent.horizontalCenter
                           LuxDomoLabel
                           {
                               width:10
                               height: parent.height
                               x:musicVolume/100*value.width
                               textName: musicVolume+"%"
                               textSize: 15
                           }
                       }
                        LuxDomoProgressBar
                        {
                            id:volume
                            width: parent.width*6/5
                            height: parent.height/2
                            y:0
                            anchors.horizontalCenter: parent.horizontalCenter
                            progressBarImage1: "../device_control/progressImage_blue.png"
                            progressBarImage2: "../device_control/progressImage.png"
                            progressButton: "../device_control/progressButton.png"
                            progressValue: musicVolume
                            enabled:musicCondition
                            onPositionchanged:
                            {
                                musicVolume=progressValue
                                pLuxDomoMusicControlItem.musicVolumeClicked(musicVolume)
                            }
                        }
                    }
                    Item
                    {
                        width: parent.width/4
                        height: parent.height
                        LuxDomoButton
                        {
                            width: parent.height*2/3
                            height: parent.height*2/3
                            anchors.horizontalCenter: parent.horizontalCenter
                            y:-parent.height/12
                            image:"../device_control/music_volume_big.png"
                            imagepressed: image
                        }
                    }
                }
            }
        }
    }
}
