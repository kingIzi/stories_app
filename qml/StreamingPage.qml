import Felgo 3.0 as Felgo
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15
import QtQuick.Controls 2.15 as QC2

Felgo.Page {
    id: _streamingPage
    property string _title
    property url video
    property url thumbnail
    //title: "Now Watching: Vuvamu TV"
    title: _videoDelegateItem.videoTitle
    QC2.ProgressBar{
        id: _streamingPageProgress
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        indeterminate: false
        visible: indeterminate
    }
    ColumnLayout{
        id: _col7
        anchors.fill: parent
        VideoPlayer{
            id: _videoPlayer
            Layout.fillWidth: true
            Layout.preferredHeight: dp(300)
            Layout.alignment: Qt.AlignTop
        }
        Item{
            Layout.fillWidth: true
            Layout.preferredHeight: 20
            Layout.margins: 10
            RowLayout{
                id: _row8
                anchors.fill: parent
                spacing: 10
                Item{
                    id: _playStop
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 20
                    Felgo.Icon{
                        icon: (video._isPlaying) ? Felgo.IconType.pause : Felgo.IconType.play
                        size: dp(20)
                        anchors.fill: parent
                    }
                    MouseArea{
                        anchors.fill: _playStop
                        onClicked: {
                            if (video._isPlaying){
                                _videoPlayer._video.pause()
                            }
                            else {
                                _videoPlayer._video.play()
                            }
                        }
                    }
                }

                Felgo.Icon{
                    icon: Felgo.IconType.stop
                    size: dp(20)
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            _videoPlayer._video.stop()
                        }
                    }
                }
                Felgo.AppText {
                    id: mediaTime
                    Layout.minimumWidth: 50
                    Layout.minimumHeight: 18
                    horizontalAlignment: Text.AlignLeft
                    text: {
                        let m = Math.floor(_videoPlayer._video.position / 60000)
                        let ms = (_videoPlayer._video.position / 1000 - m * 60)
                        let min = Math.floor((_videoPlayer._video.duration / 1000 / 60))
                        return ms.toString().substring(0,ms.toString().indexOf(".")) + " sec" + " : " + min.toString() + " min(s)"
                    }
                }
                QC2.Slider {
                    id: mediaSlider
                    enabled: _videoPlayer._video.seekable
                    Layout.fillWidth: true
                    from: 0
                    to: 1.0
                    value: _videoPlayer._video.position / _videoPlayer._video.duration
                    onMoved: _videoPlayer._video.seek(mediaSlider.value * _videoPlayer._video.duration)
                }
            }
        }
        StreamingVideoDetails{
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
