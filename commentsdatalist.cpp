#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

#include "commentsdatalist.hpp"


const QList<Comment> &CommentsDataList::comments() const
{
    return this->_comments;
}

void CommentsDataList::setComments(const QList<Comment> &newComments)
{
    this->_comments = newComments;
}

bool CommentsDataList::setItemAt(const int index, const Comment &comment)
{
    if (index < 0 || index >= this->_comments.size()) return false;

    this->_comments.insert(index,comment);
    return true;
}

QList<Comment> CommentsDataList::createCommentsList(const QJsonDocument &document)
{
    const auto videoObj = document.object().value("document").toObject();
    const auto videoComments = videoObj.value("comments").toArray().toVariantList();
    QList<Comment> comments; comments.reserve(videoComments.size());
    std::for_each(videoComments.begin(),videoComments.end(),[&comments](const QVariant& value){
        const auto map = qvariant_cast<QVariantMap>(value);
        const auto commentText = map.value("text").toString();
        const auto email = map.value("detailText").toString();
        const auto comment = Comment(commentText,email);
        comments.push_back(comment);
    });
    return comments;
}

void CommentsDataList::addComment(const Comment &newComment)
{
    emit CommentsDataList::preItemAppended();
    this->_comments.push_back(newComment);
    emit CommentsDataList::postItemAppended();
}

CommentsDataList::CommentsDataList(const QList<Comment>& comments,QObject *parent)
    : QObject{parent},
    _comments(comments)
{

}

const QString &Comment::commentText() const
{
    return this->_commentText;
}

void Comment::setCommentText(const QString &newCommentText)
{
    this->_commentText = newCommentText;
}

const QString &Comment::email() const
{
    return this->_email;
}

void Comment::setEmail(const QString &newEmail)
{
    this->_email = newEmail;
}
