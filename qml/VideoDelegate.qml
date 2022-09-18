import Felgo 3.0 as Felgo
import QtQuick 2.15
import QtQuick.Controls 2.15 as QC2
import QtQuick.Layouts 1.15


QC2.ItemDelegate {
    id: _videoDelegateItem
    signal watchVideo(var page)
    property string identifier
    property url video: "https://th.bing.com/th/id/R.3fc823bed3b952c0a3f44963e5eec2d6?rik=je%2fuiBUpy5DRDw&pid=ImgRaw&r=0"
    property url thumbnail: "https://pixabay.com/get/g8c73628ab012101a71c72f152ffdabdde147d99c7fe3b3722f7b3865d82f49a6a11bcee7b8eb09be6ca0e3c872e37ed4bf7c5e24e0fd7240ee25cf7ef48c6972_640.jpg"
    property string videoTitle
    property date released
    property var watchedUsers: []
    property var likes: []
    property var comments: []
    property bool isSaved: false
    Component{
        id: _videoCompo
        StreamingPage{
            //video: "file:///C:/Users/scott/Documents/DEVELOPMENT/c++/Felgo/stories_app/assets/manix.mp4"
            video: _videoDelegateItem.video
            //video: "qrc:/assets/manix.mp4"
            _title: _videoDelegateItem.videoTitle
            thumbnail: _videoDelegateItem.thumbnail
        }
    }
    onClicked: {
        _user.watchVideo(_videoDelegateItem.identifier,_videoDelegateItem.watchedUsers)
        _user.findVideo(identifier)
        _videoDelegateItem.watchVideo(_videoCompo)
        //_homePage.navigationStack.push(_videoCompo)
    }
    ColumnLayout{
        id: _col3
        anchors.fill: parent
        spacing: 0
        Item{
            id: _thumnailItem
            Layout.fillWidth: true
            Layout.preferredHeight: dp(200)
            Layout.alignment: Qt.AlignTop
            Image{
                anchors.fill: parent
                fillMode: Image.PreserveAspectCrop
                source: _videoDelegateItem.thumbnail
                //source: "https://th.bing.com/th/id/R.e765f106ab6b237bb3487a399fd6c75d?rik=PAaSMqDX2Mweew&pid=ImgRaw&r=0"
                asynchronous: true
            }
            Felgo.Icon {
                icon: Felgo.IconType.play
                anchors.centerIn: parent
                color: "#FFD706"
                size: dp(70)
            }
        }
        Item{
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom
            RowLayout{
                anchors.fill: parent
                anchors.margins: 10
                Felgo.AppText{
                    id: _Lbl
                    text: _videoDelegateItem.videoTitle
                    Layout.fillWidth: true
                    font.pixelSize: 48
                    font.family: "Times New Roman"
                }
                Felgo.AppText{
                    text: _videoDelegateItem.released.toLocaleDateString(Locale.ShortFormat)
                    Layout.alignment: Qt.AlignRight | Qt.AlignBottom
                    font.pixelSize: 42
                    font.family: "Times New Roman"
                    enabled: false
                    Layout.bottomMargin: 4
                }
            }
        }
    }
}
