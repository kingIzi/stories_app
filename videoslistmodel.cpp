#include <QVector>
#include "videoslistmodel.hpp"

VideosListModel::VideosListModel(QAbstractListModel *parent)
    : QAbstractListModel{parent},
      _videosDataList(nullptr)
{

}

VideosListModel::~VideosListModel()
{
    this->_videosDataList->deleteLater();
}

int VideosListModel::rowCount(const QModelIndex &index) const
{
    if (index.isValid() || !this->_videosDataList) { return 0; }
    else return this->_videosDataList->getVideosList().size();
}

QVariant VideosListModel::data(const QModelIndex &index, int role) const
{
     if (!index.isValid() || (!this->_videosDataList)) { return QVariant(); }
     const auto video = this->_videosDataList->getVideosList().at(index.row());

     switch (role){
     case IdRole:
         return video._id;
     case AdminEmailRole:
         return video._adminEmail;
     case ReleasedRole:
         return video._released;
     case TitleRole:
         return video._title;
     case WatchedUsersRole:
         return video._wacthedUsers;
     case VideoLikesRole:
         return video._videoLikes;
     case ThumbnailMetadataRole:
         return video._thumbnailMetadata;
     case VideoMetadataRole:
         return video._videoMetadata;
     case ThumbnailUrl:
         return video.getThumbnailUrl();
     case VideoUrl:
         return video.getVideoUrl();
     default:
         return QVariant();
     }
}

bool VideosListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!this->_videosDataList) { return false; }
    auto video = this->_videosDataList->getVideosList().at(index.row());

    switch (role){
    case IdRole:
        video._id = value.toString();
        break;
    case AdminEmailRole:
        video._adminEmail = value.toString();
        break;
    case ReleasedRole:
        video._released = value.toDate();
        break;
    case TitleRole:
        video._title = value.toString();
        break;
    case ThumbnailMetadataRole:
        video._thumbnailMetadata = value.toMap();
        break;
    case VideoMetadataRole:
        video._videoMetadata = value.toMap();
        break;
    default:
        return false;
    }

    if (this->_videosDataList->setItemAt(index.row(),video)){
        emit  VideosListModel::dataChanged(index,index,QVector<int>() << role);
        return true;
    }
    else return false;

}

Qt::ItemFlags VideosListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) { return Qt::NoItemFlags; }
    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> VideosListModel::roleNames() const
{
    const QHash<int, QByteArray> names = {
        {IdRole,"id"},
        {AdminEmailRole,"adminEmail"},
        {ReleasedRole,"released"},
        {TitleRole,"title"},
        {WatchedUsersRole,"watchedUsers"},
        {VideoLikesRole,"likes"},
        {ThumbnailMetadataRole,"thumbnailMetadata"},
        {VideoMetadataRole,"videoMetadata"},
        {ThumbnailUrl,"thumbnailUrl"},
        {VideoUrl,"videoUrl"},
        {CommentsRole,"comments"}
    };
    return names;
}

VideosDataList *VideosListModel::videosDataList() const
{
    return this->_videosDataList;
}

bool VideosListModel::isEmpty() const
{
    return this->_isEmpty;
}

int VideosListModel::results() const
{
    return this->_results;
}

void VideosListModel::setVideosDataList(VideosDataList *videosDataList)
{
    this->beginResetModel();

    if (this->_videosDataList){
        this->_videosDataList->disconnect(this);
        this->_videosDataList->deleteLater();
    }

    this->_videosDataList = videosDataList;

    if (this->_videosDataList){
        this->setIsEmpty(this->_videosDataList->getVideosList().isEmpty());
        this->setResults(this->_videosDataList->getVideosList().size());
        QObject::connect(this->_videosDataList,&VideosDataList::postItemAppended,this,[=](){
            const auto index = this->_videosDataList->getVideosList().size();
            this->beginInsertRows(QModelIndex(),index,index);
        });
        QObject::connect(this->_videosDataList,&VideosDataList::postItemAppended,this,[=](){
            this->endInsertRows();
        });
        QObject::connect(this->_videosDataList,&VideosDataList::preItemRemoved,this,[=](int index){
            this->beginRemoveRows(QModelIndex(),index,index);
        });
        QObject::connect(this->_videosDataList,&VideosDataList::postItemRemoved,this,[=](){
            this->endRemoveRows();
        });
    }

    this->endResetModel();
}

void VideosListModel::setIsEmpty(bool newIsEmpty)
{
    if (this->_isEmpty != newIsEmpty){
        this->_isEmpty = newIsEmpty;
        emit VideosListModel::isEmptyChanged();
    }
}

void VideosListModel::setResults(int newResults)
{
    if (this->_results != newResults){
        this->_results = newResults;
        emit VideosListModel::resultsChanged();
    }
}
