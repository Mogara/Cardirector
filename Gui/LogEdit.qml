import QtQuick 2.4

Flickable {
    property alias font: textEdit.font
    property alias text: textEdit.text
    property alias color: textEdit.color

    flickableDirection: Flickable.VerticalFlick
    contentWidth: textEdit.width
    contentHeight: textEdit.height
    clip: true

    TextEdit {
        id: textEdit

        clip: true
        readOnly: true
        selectByKeyboard: true
        selectByMouse: true
        wrapMode: TextEdit.WordWrap
    }

    function append(text)
    {
        var autoScroll = atYEnd;
        textEdit.append(text);
        if (autoScroll && contentHeight > contentY + height) {
            contentY = contentHeight - height;
        }
    }
}
