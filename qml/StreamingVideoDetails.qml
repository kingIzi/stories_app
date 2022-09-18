import Felgo 3.0 as Felgo
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QC2
import CommentsList 1.0

Item{
    Connections{
        target: _user
        function onNewVideo(video){
            _streamingPageProgress.indeterminate = false
            if (video.likes){
                let isLiked = video.likes.includes(_user.getLocalId())
                _thumbsUpIcon.color = (isLiked) ? "red" : "black"
            }
            if (video.watchedUsers){
                _videoDelegateItem.watchedUsers = video.watchedUsers
            }
            if (video.comments){
                console.log(video.comments)
            }
        }
        function onVideoComments(comments){
            _streamingPageProgress.indeterminate = false
            _commentsListModel.setCommentsList(comments)
        }
        function onFavoritesChanged(){
            _streamingPageProgress.indeterminate = false
            _heartIcon._isFavorite = _user.favorites.includes(identifier)
        }
    }
    Component.onCompleted: {
        _user.findVideoComments(identifier)
    }

    Felgo.AppFlickable{
        id: flick3
        anchors.fill: parent
        contentHeight: _col10.implicitHeight
        ColumnLayout{
            id: _col10
            anchors.fill: parent
            spacing: 10
            Item{
                Layout.fillWidth: true
                Layout.preferredHeight: _row2.implicitHeight
                Layout.bottomMargin: 10
                RowLayout{
                    id: _row2
                    anchors.fill: parent
                    Flow{
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignLeft
                        flow: Flow.TopToBottom
                        leftPadding: 8
                        spacing: 8
                        Text{
                            text:  _videoDelegateItem.videoTitle
                            font.pixelSize: 42
                            font.family: "Times New Roman"
                        }
                        Text{
                            text: _videoDelegateItem.watchedUsers.length + " Batala"
                            color: "grey"
                            font.pixelSize: 32
                            font.family: "Times New Roman"
                        }
                    }
                    Text{
                        text: _videoDelegateItem.released.toLocaleString(Locale.ShortFormat)
                        color: "grey"
                        font.pixelSize: 32
                        font.family: "Times New Roman"
                        Layout.alignment: Qt.AlignRight | Qt.AlignBottom
                    }
                }
            }
            Item{
                Layout.fillWidth: true
                Layout.preferredHeight: _row1.implicitHeight
                RowLayout{
                    id: _row1
                    anchors.fill: parent
                    spacing: 8
                    Item{
                        Layout.fillWidth: true
                        Layout.preferredHeight: _col11.implicitHeight
                        ColumnLayout{
                            id: _col11
                            anchors.fill: parent
                            Felgo.Icon{
                                id: _thumbsUpIcon
                                icon: Felgo.IconType.thumbsup
                                size: dp(50)
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Felgo.AppText{
                                text: "O lingi"
                                Layout.alignment: Qt.AlignHCenter
                                horizontalAlignment: "AlignHCenter"
                            }
                        }
                        MouseArea{
                            anchors.fill: parent
                            cursorShape: "PointingHandCursor"
                            onClicked: {
                                _streamingPageProgress.indeterminate = true
                                _user.likeVideo(identifier)
                            }
                        }
                    }
                    Item{
                        Layout.fillWidth: true
                        Layout.preferredHeight: _col12.implicitHeight
                        ColumnLayout{
                            id: _col12
                            anchors.fill: parent
                            Felgo.Icon{
                                id: _heartIcon
                                property bool _isFavorite: _user.favorites.includes(identifier)
                                icon: Felgo.IconType.heart
                                size: dp(50)
                                Layout.alignment: Qt.AlignHCenter
                                color: _isFavorite ? "red" : "black"
                            }
                            Felgo.AppText{
                                text: "Batela oyo"
                                Layout.alignment: Qt.AlignHCenter
                                horizontalAlignment: "AlignHCenter"
                            }
                        }
                        MouseArea{
                            anchors.fill: parent
                            cursorShape: "PointingHandCursor"
                            onClicked: {
                                _streamingPageProgress.indeterminate = true
                                _user.addFavoriteVideo(identifier)
                            }
                        }
                    }
                }
            }
            Item{
                Layout.preferredHeight: 400
                Layout.fillWidth: true
                ColumnLayout{
                    anchors.fill: parent
                    Felgo.AppText{
                        text: "Commentaires"
                        padding: 8
                        font.pixelSize: 62
                        font.family: "Times New Roman"
                        Layout.fillWidth: true
                    }
                    QC2.TextArea{
                        id: _commentText
                        Layout.fillWidth: true
                        placeholderText: "Koma Commentaire awa"
                        font.pixelSize: 42
                        font.family: "Times New Roman"
                        padding: 8
                        wrapMode: "WordWrap"
                    }
                    Felgo.AppButton{
                        text: "Commenter"
                        Layout.alignment: Qt.AlignRight
                        onClicked: {
                            _streamingPageProgress.indeterminate = true
                            let comments = _videoDelegateItem.comments
                            if (comments){
                                _user.addComment(identifier,_commentText.text,comments)
                            }
                            else {
                                _user.addComment(identifier,_commentText.text,[])
                            }

                        }
                    }
                    QC2.ToolSeparator{
                        orientation: "Horizontal"
                        Layout.fillWidth: true
                    }
                    Item{
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        ListView {
                            anchors.fill: parent
                            interactive: false
                            delegate: Felgo.SimpleRow {
                                text: model.text
                                detailText: model.detailText
                            }
                            model: CommentsListModel{
                                id: _commentsListModel
                            }
                        }
                    }
                }
            }
        }
    }
}
