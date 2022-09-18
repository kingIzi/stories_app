#pragma once

#include <QObject>
#include <QAbstractListModel>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QVariantList>

#include "commentsdatalist.hpp"

class CommentsListModel : public QAbstractListModel{
    Q_OBJECT
    Q_PROPERTY(CommentsDataList* commentsList READ commentsList WRITE setCommentsList NOTIFY commentsListChanged);
private:
    CommentsDataList* _commentsList;
public:
    explicit CommentsListModel(QAbstractListModel *parent = nullptr);
    ~CommentsListModel();
    enum{
        CommentRole = Qt::UserRole,
        EmailRole
    };
    int rowCount(const QModelIndex& index = QModelIndex()) const override;
    QVariant data(const QModelIndex& index,int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index,const QVariant& value,int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    virtual QHash<int,QByteArray> roleNames() const override;
    CommentsDataList* commentsList() const;
public slots:
    void setCommentsList(CommentsDataList* commentsList);
signals:
    void commentsListChanged();
};

