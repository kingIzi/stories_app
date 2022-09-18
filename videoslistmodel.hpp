#pragma once

#include <QObject>
#include <QAbstractListModel>
#include <QByteArray>
#include <QHash>

#include "videodatalist.hpp"

class VideosListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(VideosDataList* videosDataList READ videosDataList WRITE setVideosDataList NOTIFY videosDataListChanged);
    Q_PROPERTY(bool isEmpty READ isEmpty WRITE setIsEmpty NOTIFY isEmptyChanged);
    Q_PROPERTY(int results READ results WRITE setResults NOTIFY resultsChanged);
private:
    VideosDataList* _videosDataList;
    bool _isEmpty = true;
    int _results = 0;
public:
    explicit VideosListModel(QAbstractListModel *parent = nullptr);
    ~VideosListModel();
    using VideosList = VideosDataList;
    enum{
        IdRole = Qt::UserRole,
        AdminEmailRole,
        ReleasedRole,
        TitleRole,
        WatchedUsersRole,
        VideoLikesRole,
        ThumbnailMetadataRole,
        VideoMetadataRole,
        ThumbnailUrl,
        VideoUrl,
        CommentsRole
    };

    int rowCount(const QModelIndex& index = QModelIndex()) const override;
    QVariant data(const QModelIndex& index,int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index,const QVariant& value,int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    virtual QHash<int,QByteArray> roleNames() const override;
    VideosDataList* videosDataList() const;
    bool isEmpty() const;
    int results() const;
public slots:
    void setVideosDataList(VideosDataList* videosDataList);
    void setIsEmpty(bool newIsEmpty);
    void setResults(int newResults);
signals:
    void videosDataListChanged();
    void isEmptyChanged();
    void resultsChanged();
};

class FavoriteListModel : public VideosListModel{
    Q_OBJECT
};

