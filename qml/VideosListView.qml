import Felgo 3.0 as Felgo
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QC2
import VideosList 1.0

//Item{
//    id: _videosListItem
//    signal openStreamingPage(var page)
//    property VideosListModel _listModel: VideosListModel{}
//    property Component _videoDelegate: Component{
//        VideoDelegate{
//                id: _videoItem
//                identifier: model.id
//                video: model.videoUrl
//                thumbnail: model.thumbnailUrl
//                videoTitle: model.title
//                released: model.released
//                watchedUsers: model.watchedUsers
//                likes: model.likes
//                comments: model.comments
//                Connections{
//                    target: _videoItem
//                    function onWatchVideo(page){
//                        _videosListItem.openStreamingPage(page)
//                    }
//                }
//            }
//    }

//    QC2.ScrollView{
//        id: _homeScroll
//        anchors.fill: parent
//        clip: true
//        Column{
//            anchors.fill: parent
//            ListView{
//                id: _homeVideosList
//                width: parent.width
//                height: parent.height
//                spacing: 10
////                delegate: _videoDelegate
////                model: VideosListModel{
////                    id: _videosListModel
////                }
//                model: _videosListItem._listModel
//                delegate: _videoDelegate
//                footer: Item{
//                    id: _resultsItem
//                    width: parent.width
//                    height: dp(50)
//                    RowLayout{
//                        anchors.fill: parent
//                        Rectangle{
//                            Layout.fillWidth: true
//                            Layout.preferredHeight: 1
//                            color: "grey"
//                        }
//                        QC2.Label{
//                            //text: "Ozomona bilili " + _videosListModel.results
//                            text: "Ozali komana bilili " + _videosListItem._listModel.results
//                            font.pixelSize: dp(24)
//                        }
//                        Rectangle{
//                            Layout.fillWidth: true
//                            Layout.preferredHeight: 1
//                            color: "grey"
//                        }
//                    }
//                }
//            }
//        }
//    }
//}


Item{
    id: _item4
    readonly property alias _model: _videosListModel
    signal openStreamingPage(var streamingPage)
    Component{
        id: _videoDelegate
        VideoDelegate{
            id: _delegate
            width: _item4.width
            height: dp(250)
            Connections{
                target: _delegate
                function onWatchVideo(page){
                    _item4.openStreamingPage(page)
                }
            }
            identifier: model.id
            video: model.videoUrl
            thumbnail: model.thumbnailUrl
            videoTitle: model.title
            released: model.released
            watchedUsers: model.watchedUsers
            likes: model.likes
            comments: model.comments
        }
    }
    QC2.ScrollView{
        id: _homeScroll
        anchors.fill: parent
        clip: true
        Column{
            anchors.fill: parent
            ListView{
                id: _homeVideosList
                width: parent.width
                height: parent.height
                spacing: 10
                delegate: _videoDelegate
                model: VideosListModel{
                    id: _videosListModel
                }
                footer: Item{
                    id: _resultsItem
                    width: parent.width
                    height: dp(50)
                    RowLayout{
                        anchors.fill: parent
                        Rectangle{
                            Layout.fillWidth: true
                            Layout.preferredHeight: 1
                            color: "grey"
                        }
                        QC2.Label{
                            text: "Ozomona bilili " + _videosListModel.results
                            font.pixelSize: dp(24)
                        }
                        Rectangle{
                            Layout.fillWidth: true
                            Layout.preferredHeight: 1
                            color: "grey"
                        }
                    }
                }
            }
        }
    }
}
