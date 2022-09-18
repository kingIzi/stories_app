import Felgo 3.0 as Felgo
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QC2
import QtQuick.Dialogs 1.3
import VideosList 1.0

QC2.Drawer{
    id: _uploadNewVideoDrawer
    edge: Qt.BottomEdge
    background: Rectangle{
        radius: dp(20)
        Rectangle{
            width: parent.width
            height: parent.radius
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: parent.left
        }
    }
    onAboutToShow: {
        _user.getAllVideos()
    }
    Connections{
        target: _user
        function onAvailableVideos(videos){
            _videosModel.setVideosDataList(videos)
        }
    }
    FileDialog {
        id: _imgFileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        nameFilters: [ "Image files (*.jpg *.png *.jpeg)", "All files (*)" ]
    }
    FileDialog {
        id: _videoFileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        nameFilters: [ "Image files (*.mp4 *.avi *.wav)", "All files (*)" ]
    }
    VideosListModel{
        id: _videosModel
    }
    Component{
        id: _options
        Item{
            Felgo.AppListView{
                anchors.fill: parent
                model: [{"color": "#000","label":"Batisa Video","detail":"Fina awa pona kobakisa video"},
                        {"color": "red","label":"Longola Video","detail":"Fina awa pona kolongola video"}]
                delegate: Felgo.AppListItem{
                    text: modelData.label
                    textColor: modelData.color
                    detailText: modelData.detail
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            switch (index){
                            case 0:
                                _uploadStack.push(_addVideoComponent)
                                break
                            case 1:
                                _uploadStack.push(_removeOptions)
                                break
                            }
                        }
                    }
                }
            }
        }
    }
    Component{
        id: _removeOptions
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
                                text: "Longola Video"
                                font.pixelSize: 42
                                font.family: "Times New Roman"
                                Layout.alignment: Qt.AlignRight
                                horizontalAlignment: "AlignHCenter"
                            }
                        }
                    }
                    Item{
                        Layout.preferredHeight: dp(600)
                        Layout.fillWidth: true
                        Felgo.AppListView{
                            id: _appList
                            anchors.fill: parent
                            spacing: 32
                            model: _videosModel
                            delegate: Felgo.AppListItem{
                                id: _item5
                                readonly property string _id: model.id
                                readonly property string _title: model.title
                                readonly property string _adminEmail: model.adminEmail
                                readonly property string _thumbnail: model.thumbnailUrl
                                text: _item5._title
                                detailText: _item5._adminEmail
                                width: _appList.width
                                height: dp(80)
                                rightItem: Image{
                                    source: _item5._thumbnail
                                    fillMode: Image.PreserveAspectFit
                                    sourceSize.width: dp(40)
                                    sourceSize.height: dp(20)
                                }
                                MouseArea{
                                    anchors.fill: parent
                                    cursorShape: "PointingHandCursor"
                                    onClicked: {
                                        _user.removeVideo(_item5._id)
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    Component{
        id: _addVideoComponent
        AddVideo{}
    }
    QC2.StackView{
       id: _uploadStack
       anchors.fill: parent
       initialItem: _options
    }
}
