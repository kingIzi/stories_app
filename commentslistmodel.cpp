#include "commentslistmodel.hpp"

CommentsListModel::CommentsListModel(QAbstractListModel *parent) :
    QAbstractListModel{parent},
    _commentsList(nullptr)
{

}

CommentsListModel::~CommentsListModel()
{
    this->_commentsList->deleteLater();
}

int CommentsListModel::rowCount(const QModelIndex &index) const
{
    if (index.isValid() || !this->_commentsList) { return 0; }
    return this->_commentsList->comments().size();
}

QVariant CommentsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !this->_commentsList) { return QVariant(); }
    const auto comment = this->_commentsList->comments().at(index.row());

    switch (role){
    case CommentRole:
        return comment._commentText;
    case EmailRole:
        return comment._email;
    default:
        return QVariant();
    }
}

bool CommentsListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!this->_commentsList) { return false; }
    auto comment = this->_commentsList->comments().at(index.row());

    switch (role){
    case CommentRole:
        comment._commentText = value.toString();
        return true;
    case EmailRole:
        comment._email = value.toString();
        return true;
    default:
        return false;
    }
}

Qt::ItemFlags CommentsListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) { return Qt::NoItemFlags; }
    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> CommentsListModel::roleNames() const
{
    const QHash<int, QByteArray> names = {
        {CommentRole,"text"},
        {EmailRole,"detailText"}
    };
    return names;
}

CommentsDataList *CommentsListModel::commentsList() const
{
    return this->_commentsList;
}

void CommentsListModel::setCommentsList(CommentsDataList *commentsList)
{
    this->beginResetModel();
    if (this->_commentsList){
        this->_commentsList->disconnect(this);
        this->_commentsList->deleteLater();
    }
    this->_commentsList = commentsList;

    if (this->_commentsList){
        QObject::connect(this->_commentsList,&CommentsDataList::postItemAppended,this,[this](){
            const auto index = this->_commentsList->comments().size();
            this->beginInsertRows(QModelIndex(),index,index);
        });
        QObject::connect(this->_commentsList,&CommentsDataList::postItemAppended,this,[this](){
            this->endInsertRows();
        });
        QObject::connect(this->_commentsList,&CommentsDataList::preItemRemoved,this,[this](int index){
            this->beginRemoveRows(QModelIndex(),index,index);
        });
        QObject::connect(this->_commentsList,&CommentsDataList::postItemRemoved,this,[this](){
            this->endRemoveRows();
        });
    }
    this->endResetModel();
}
