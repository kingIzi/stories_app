import Felgo 3.0 as Felgo
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QC2

Felgo.Page{
    id: _homePage
    title: "Boyebi na biso"
    readonly property alias progress: _homePageProgress
    rightBarItem: Felgo.IconButtonBarItem{
        icon: Felgo.IconType.plus
        onClicked: _uploadVideoDrawer.open()
        visible: _user.getUserType() === "MOKOTISI"
    }
//    Component.onCompleted: {
//        console.log("component on completed VideoListModel")
//        _user.selectAllVideos()
//    }
//    onVisibleChanged: {
//        if (visible){
//            console.log("videos changed VideoListModel")
//            _user.selectAllVideos()
//        }
//    }
    QC2.ProgressBar{
        id: _homePageProgress
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        indeterminate: true
        visible: indeterminate
    }
    UploadNewVideoDrawer{
        id: _uploadVideoDrawer
        width: parent.width
        height: dp(600)
    }
    Connections{
        target: _user
        function onAllVideos(videos){
            progress.indeterminate = false
            console.log(videos)
            _videosListView._model.setVideosDataList(videos)
            //_videosListView._listModel.setVideosDataList(videos)
        }
    }
    Connections{
        target: _videosListView
        function onOpenStreamingPage(streamingPage){
            _homePage.navigationStack.push(streamingPage)
        }
    }
    VideosListView{
        id: _videosListView
        width: parent.width
        height: parent.height
    }
}

