import QtQuick 1.0
Item {
    property string name: localfont.name
    FontLoader {
        id:localfont
        source: "file:///mnt/disk/lib/fonts/fzlth.ttf"
    }
}

