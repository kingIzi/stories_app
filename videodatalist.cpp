#include <QDebug>
#include "videodatalist.hpp"


VideosDataList::VideosDataList(const QList<Video>& videos,QObject *parent)
    : QObject{parent},
      _videosList(videos)
{

}

Video VideosDataList::createVideo(const QVariantMap &video)
{
    const auto getValue = [video](const QString key){ return video.value(key); };

    const auto id = getValue("_id").toString();
    const auto adminEmail = getValue("adminEmail").toString();
    const auto released = getValue("released").toDate();
    const auto title = getValue("title").toString();
    const auto watchedUsers = static_cast<QVariantList>(getValue("watchedUsers").toList());
    const auto videoLikes = static_cast<QVariantList>(getValue("likes").toList());
    const auto thumbnailMetadata = getValue("thumbnailMetadata").toMap();
    const auto videoMetadata = getValue("videoMetadata").toMap();

    return Video(id,adminEmail,released,title,thumbnailMetadata,videoMetadata,watchedUsers,videoLikes);
}

QList<Video> VideosDataList::createVideosList(const QJsonDocument &documents)
{
    const auto videosArray = documents.object().value("documents").toArray();
    QList<Video> videos; videos.reserve(videosArray.size());
    std::for_each(videosArray.begin(),videosArray.end(),[&videos](const QJsonValue& value){
        const auto videoObj = value.toObject().toVariantMap();
        const auto video = VideosDataList::createVideo(videoObj);
        videos.push_back(video);
    });
    return videos;
}

QList<Video> VideosDataList::createFavoriteVideosList(const QJsonDocument &documents, const QStringList &favorites)
{
    auto videosArray = documents.object().value("documents").toArray();
    QList<Video> videos; videos.reserve(videosArray.size());
    for (auto i = 0; i < videosArray.size(); i++){
        const auto videoObj = videosArray[i].toObject().toVariantMap();
        const auto videoId = videoObj.value("_id").toString();
        if (favorites.contains(videoId)) {
            const auto video = VideosDataList::createVideo(videoObj);
            videos.push_back(video);
        }
    }
    return videos;
}

const QList<Video> &VideosDataList::getVideosList() const
{
    return this->_videosList;
}

QList<Video> VideosDataList::getVideosListModifiable()
{
    return this->_videosList;
}

void VideosDataList::setVideosList(const QList<Video> &newVideoList)
{
    this->_videosList = newVideoList;
}

bool VideosDataList::setItemAt(const int index, const Video &video)
{
    if (index < 0 || index >= this->_videosList.size()) return false;

    const auto oldItem = this->_videosList.at(index);
    if (oldItem._id.compare(video._id) == 0) { return false; }

    this->_videosList.insert(index,video);
    return true;
}

void VideosDataList::addVideo(const Video &newVideo)
{
    emit VideosDataList::preItemAppended();
    this->_videosList.push_back(newVideo);
    emit VideosDataList::postItemAppended();
}

const QVariantMap &Video::thumbnailMetadata() const
{
    return this->_thumbnailMetadata;
}

void Video::setThumbnailMetadata(const QVariantMap &newThumbnailMetadata)
{
    this->_thumbnailMetadata = newThumbnailMetadata;
}

const QString &Video::title() const
{
    return this->_title;
}

void Video::setTitle(const QString &newTitle)
{
    this->_title = newTitle;
}

const QDate &Video::released() const
{
    return this->_released;
}

void Video::setReleased(const QDate &newReleased)
{
    this->_released = newReleased;
}

const QString &Video::adminEmail() const
{
    return this->_adminEmail;
}

void Video::setAdminEmail(const QString &newAdminEmail)
{
    this->_adminEmail = newAdminEmail;
}

const QString &Video::id() const
{
    return this->_id;
}

void Video::setId(const QString &newId)
{
    this->_id = newId;
}

const QVariantList &Video::getComments() const
{
    return this->_comments;
}

void Video::setComments(const QVariantList &newComments)
{
    this->_comments = newComments;
}

const QVariantMap &Video::videoMetadata() const
{
    return this->_videoMetadata;
}

void Video::setVideoMetadata(const QVariantMap &newVideoMetadata)
{
    this->_videoMetadata = newVideoMetadata;
}
