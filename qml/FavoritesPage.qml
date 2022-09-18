import Felgo 3.0 as Felgo
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QC2
import FavoriteList 1.0


Felgo.Page {
    id: _favoritesPage
    title: "Vos favori"
    Connections{
        target: _user
        function onFavoriteVideos(favorites){
            _favoritesPageProgress.indeterminate = false
            _videosListView2._model.setVideosDataList(favorites)
        }
    }
//    Component.onCompleted: {
//        console.log("favorites component on completed VideoListModel")
//        _user.selectFavoriteVideos()
//    }
    QC2.ProgressBar{
        id: _favoritesPageProgress
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        indeterminate: true
        visible: indeterminate
    }
    Felgo.AppText{
        id: _emptyText
        text: "Nanu bo kotisi ba favori na bino te"
        font.pixelSize: 42
        anchors.centerIn: parent
        visible: _videosListView2._model.isEmpty
    }
    Connections{
        target: _videosListView2
        function onOpenStreamingPage(streamingPage){
            _favoritesPage.navigationStack.push(streamingPage)
        }
    }
    Connections{
        target: _user
        function onFavoriteVideos(favorites){
            _videosListView2._model.setVideosDataList(favorites)
            //_favoriteListModel.setVideosDataList(favorites)
        }
    }

//    Item{
//        anchors.fill: parent
//        width: parent.width
//        height: parent.height
//        ListView{
//            model: FavoriteListModel{
//                id: _favoriteListModel
//            }
//            delegate: VideoDelegate{
//                id: _delegate
//                Connections{
//                    target: _delegate
//                    function onWatchVideo(page){
//                        _item4.openStreamingPage(page)
//                    }
//                }
//                identifier: model.id
//                video: model.videoUrl
//                thumbnail: model.thumbnailUrl
//                videoTitle: model.title
//                released: model.released
//                watchedUsers: model.watchedUsers
//                likes: model.likes
//                comments: model.comments
//            }
//            anchors.fill: parent
//        }
//    }
    VideosListView{
        id: _videosListView2
        width: parent.width
        height: parent.height
        visible: !_emptyText.visible
    }
}
