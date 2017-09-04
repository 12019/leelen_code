import QtQuick 1.0
Rectangle
{
    id:button
    width: 150
    height: 100
    radius: 10

    signal clicked
    signal pressed
    signal released
    signal cancel
    signal positionchanged
    property string name: ""  //按键名
    property int fontsize: 100  //字体大小
    property string image: ""   //按键图片
    property string imagepressed: "" //按下
    property string textcolor: "#ffffff" //字体颜色
    property int mousex

    LocalFontLoader{
        id:localfont
    }

    Image {
        id:bg
        source: button_mouse.pressed?imagepressed:image
        anchors.fill: parent
        opacity: 1
        TextEdit
        {
            opacity: 1
            color: button.textcolor
            anchors.fill: parent
            text: button.name
            font.pixelSize: fontsize
            font.family: localfont.name
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            activeFocusOnPress: false
            readOnly: true
            focus: false
            cursorVisible: false
        }
        MouseArea{
            id:button_mouse
            anchors.fill: parent
            onClicked: button.clicked()
            onPressed: {
                mousex = mouseX
                button.pressed()
            }
            onReleased: button.released()
            onCanceled: button.cancel()
            onPositionChanged: {
                mousex = mouseX
                button.positionchanged()
            }
        }
    }
}
