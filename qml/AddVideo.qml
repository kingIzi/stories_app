import Felgo 3.0 as Felgo
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QC2
import QtQuick.Dialogs 1.3

Item{
    Felgo.AppFlickable{
        anchors.fill: parent
        contentHeight: _col15.implicitHeight
        ColumnLayout{
            id: _col15
            anchors.fill: parent
            anchors.margins: 10
            spacing: 72
            Item{
                Layout.fillWidth: true
                Layout.preferredHeight: dp(80)
                RowLayout{
                    anchors.fill: parent
                    Felgo.Icon{
                        Layout.alignment: Qt.AlignLeft
                        icon: Felgo.IconType.arrowleft
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                _uploadStack.pop()
                            }
                        }
                    }

                    Felgo.AppText{
                        text: "Bakisa Video"
                        font.pixelSize: 42
                        font.family: "Times New Roman"
                        Layout.alignment: Qt.AlignRight
                        horizontalAlignment: "AlignHCenter"
                    }
                }
            }

            Felgo.TextFieldRow {
                id: _videoTitleInput
                labelWidth: dp(74)
                label: qsTr("Titre ya video")
                placeHolder: qsTr("Koma titre ya video")
                textFieldItem.inputMode: textFieldItem.inputModeUsername
                Layout.fillWidth: true
            }
            Felgo.TextFieldRow {
              id: birthField
              Layout.fillWidth: true
              Layout.preferredHeight: 40
              label: qsTr("Date Published")
              placeHolder: qsTr("Enter your birth date")
              // instead of requiring the user to manually type a date we just show a native date picker when he/she taps on it.
              clickEnabled: true
              onClicked: nativeUtils.displayDatePicker()
              value: new Date().toLocaleDateString(Locale.ShortFormat)
              enabled: false
              // When accepted, set displayed date to selected value
              Connections {
                target: nativeUtils
                onDatePickerFinished: {
                  if (accepted){
                    birthField.value = new Date(date).toLocaleDateString(Locale.ShortFormat)
                  }
                }
              }
            }
            Item{
                Layout.fillWidth: true
                Layout.preferredHeight: _row4.implicitHeight
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                RowLayout{
                    id: _row4
                    anchors.fill: parent
                    Felgo.AppText{
                        text: "Thumbnail ya video"
                        font.pixelSize: 38
                        font.family: "Times New Roman"
                        Layout.fillWidth: true
                    }
                    Felgo.SimpleButton{
                        Layout.alignment: Qt.AlignRight
                        text: "Browse"
                        onClicked: _imgFileDialog.open()
                        font.pixelSize: 42
                    }
                }
            }
            Item{
                Layout.fillWidth: true
                Layout.preferredHeight: _row5.implicitHeight
                Layout.leftMargin: 10
                Layout.rightMargin: 10
                RowLayout{
                    id: _row5
                    anchors.fill: parent
                    Felgo.AppText{
                        text: "Video"
                        font.pixelSize: 38
                        font.family: "Times New Roman"
                        Layout.fillWidth: true
                    }
                    Felgo.SimpleButton{
                        Layout.alignment: Qt.AlignRight
                        text: "Browse"
                        onClicked: _videoFileDialog.open()
                        font.pixelSize: 42
                    }
                }
            }
            Felgo.AppButton{
                text: "Bakisa Video"
                Layout.alignment: Qt.AlignHCenter
                onClicked: {
                    const video = {
                        "title": _videoTitleInput.value,
                        "released": new Date(birthField.value),
                        "thumbnailMetadata": _imgFileDialog.fileUrl,
                        "videoMetadata": _videoFileDialog.fileUrl
                    };
                    user.addNewVideo(video)
                }
            }
        }
    }
}
