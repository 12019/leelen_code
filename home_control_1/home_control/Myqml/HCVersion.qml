import QtQuick 1.0

Item {
    id: version
    width: 800
    height: 480

    LocalFontLoader{
        id:localfont
    }

    TextEdit {
        id: voldVersionText
        height: 50
        text: m_pHCSetting.getHomeVersionText()
        anchors.left: parent.left
        anchors.top: parent.top
        activeFocusOnPress: false
        readOnly: true
        anchors.verticalCenterOffset: 0
        font.pixelSize: 30
        font.family: localfont.name
    }

    TextEdit {
        id: newVersionText
        height: 50
        text: m_pHCSetting.getNewVersionText()
        anchors.left: parent.left
        anchors.top: voldVersionText.bottom
        activeFocusOnPress: false
        readOnly: true
        anchors.verticalCenterOffset: 0
        font.pixelSize: 30
        font.family: localfont.name
    }

   Rectangle{
          id:updateVersion
          height: 50
          width: 500
           anchors.verticalCenter: parent.verticalCenter
           visible: m_pHCSetting.getIsUpdate()
           TextEdit{
                  text:m_pHCSetting.getUpdateVersionText()
                  anchors.fill: updateVersion
                  font.pixelSize: 30
                  font.family: localfont.name
           }
           MouseArea{
                   anchors.fill: updateVersion
                   onClicked: m_pHCSetting.downloadNewVersion()
           }
   }
}
