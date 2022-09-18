import Felgo 3.0 as Felgo
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15
import QtQuick.Controls 2.15 as QC2

Item{
    id: _videoRect
    readonly property alias _video: video
    Image{
        visible: _playIcon.visible
        fillMode: Image.PreserveAspectCrop
        source: _streamingPage.thumbnail
        anchors.fill: _videoRect
    }
    Felgo.Icon {
        id: _playIcon
        icon: Felgo.IconType.play
        anchors.centerIn: parent
        color: "#FFD706"
        size: dp(70)
        visible: video.playbackState !== MediaPlayer.PlayingState
        MouseArea {
            anchors.fill: parent
            onClicked: {
                video.play()
            }
        }
    }
    Video {
        readonly property bool _isPlaying: video.playbackState === MediaPlayer.PlayingState
        id: video
        anchors.fill: parent
        visible: !_playIcon.visible
        //source: _streamingPage.video
        source: "file:///C:/Users/scott/OneDrive/Documents/Development/c++/kimpaVita.mp4"
        focus: true
        autoPlay: true
        Keys.onSpacePressed: (video._isPlaying) ? video.pause() : video.play()
        Keys.onLeftPressed: video.seek(video.position - 5000)
        Keys.onRightPressed: video.seek(video.position + 5000)
    }
    MouseArea{
        anchors.fill: parent
        cursorShape: "PointingHandCursor"
        onClicked: {
            if (video.playbackState === MediaPlayer.PlayingState){
                video.pause()
            }
            else{
                video.play()
            }
        }
    }
}
