#pragma once

#include <QVariantMap>
#include <QObject>
#include <QList>
#include <QString>
#include <QUrl>
#include <QUrlQuery>
#include <QDate>
#include <QVariantList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>



struct Video{
    QString _id;
    QString _adminEmail;
    QDate _released;
    QString _title;
    QVariantList _comments;
    QVariantMap _thumbnailMetadata;
    QVariantMap _videoMetadata;
    QVariantList _wacthedUsers;
    QVariantList _videoLikes;
    Video() = default;
    Video(const QString& id,const QString& adminEmail,const QDate& released,const QString& title,
          const QVariantMap& thumbnailMetadata,const QVariantMap& videoMetadata,
          const QVariantList& watchedUsers = QVariantList(), const QVariantList& videoLikes = QVariantList()) :
        _id(id),_adminEmail(adminEmail),_released(released),_title(title),_thumbnailMetadata(thumbnailMetadata),
        _videoMetadata(videoMetadata),_wacthedUsers(watchedUsers),_videoLikes(videoLikes)

    {}
    const QUrl getUrlFromMetadata(const QVariantMap& metadata) const{
        auto contentDisposition = metadata.value("contentDisposition").toString();
        contentDisposition.remove(0,25);
        const auto downloadTokens = metadata.value("downloadTokens").toString();
        const auto endpoint = Video::firebaseStorage + contentDisposition;
        QUrlQuery query;
        query.addQueryItem("alt","media"); query.addQueryItem("token",downloadTokens);
        QUrl url(endpoint);
        url.setQuery(query);
        return url;
    }
    const QUrl getThumbnailUrl() const{
        const auto thumbnailUrl = this->getUrlFromMetadata(this->_thumbnailMetadata);
        return thumbnailUrl;
    }
    const QUrl getVideoUrl() const{
        const auto videoUrl = this->getUrlFromMetadata(this->_videoMetadata);
        return videoUrl;
    }
    static constexpr const char* firebaseStorage = "https://firebasestorage.googleapis.com/v0/b/kongo-history.appspot.com/o/";
    bool operator==(const Video& video){
        return this->_id.compare(video._id) == 0;
    }

public:
    const QVariantMap &videoMetadata() const;
    void setVideoMetadata(const QVariantMap &newVideoMetadata);
    const QVariantMap &thumbnailMetadata() const;
    void setThumbnailMetadata(const QVariantMap &newThumbnailMetadata);
    const QString &title() const;
    void setTitle(const QString &newTitle);
    const QDate &released() const;
    void setReleased(const QDate &newReleased);
    const QString &adminEmail() const;
    void setAdminEmail(const QString &newAdminEmail);
    const QString &id() const;
    void setId(const QString &newId);
    const QVariantList &getComments() const;
    void setComments(const QVariantList &newComments);
};

class VideosDataList : public QObject
{
    Q_OBJECT
private:
    QList<Video> _videosList;
public:
    explicit VideosDataList(const QList<Video>& videos,QObject *parent = nullptr);
    static Video createVideo(const QVariantMap& video);
    static QList<Video> createVideosList(const QJsonDocument& document);
    static QList<Video> createFavoriteVideosList(const QJsonDocument& document,const QStringList& favorites);
    const QList<Video> &getVideosList() const;
    QList<Video> getVideosListModifiable();
    void setVideosList(const QList<Video> &newVideoList);
    bool setItemAt(const int index,const Video& video);
public slots:
    void addVideo(const Video& newVideo);
signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();
};

