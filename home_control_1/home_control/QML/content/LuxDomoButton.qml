import QtQuick 1.0
Rectangle
{
    id:button
    width: 150
    height: 100
    radius: 10
    color: buttonColor
    border.color: button.borderColor
    border.width: button.borderWidth

    signal  clicked
    signal  pressed
    signal  released
    signal  cancel
    signal  positionchanged
    signal  pressAndHold
    property string name: ""  //按键名
    property int fontsize: 20  //字体大小
    property string image:"../home_manage/button_background.png"  //按键图片
    property string imagepressed: "../home_manage/button_background1.png" //按下
    property string textcolor: "#151412"
    property string buttonColor: "#00000000"
    property string borderColor: "#00000000"
    property int borderWidth: 0
    property bool   showImage: true

    LuxDomoFontLoader
    {
         id:localfont
    }

    Image
    {
        id:buttonImage
        visible: showImage
        source: buttonMouse.pressed?imagepressed:image
        anchors.fill: button
      }

        TextEdit
        {
            id: text
            anchors.horizontalCenter: button.horizontalCenter
            anchors.verticalCenter: button.verticalCenter
            font.family: localfont.name
            text: button.name
            color: button.textcolor
            font.pixelSize: button.fontsize
        }

        MouseArea{
            id:buttonMouse
            anchors.fill: button
            onClicked: button.clicked()
            onPressed: button.pressed()
            onReleased: button.released()
            onCanceled: button.cancel()
            onPositionChanged: button.positionchanged()
            onPressAndHold:button.pressAndHold()
        }   
}
