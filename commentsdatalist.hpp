#pragma once

#include <QObject>
#include <QList>
#include <QString>

struct Comment{
    QString _commentText;
    QString _email;
public:
    bool operator==(const Comment& comment){
        const auto matchComment = this->_commentText.compare(comment._commentText) == 0;
        const auto matchEmail = this->_email.compare(comment._email) == 0;
        return matchComment && matchEmail;
    }
public:
    Comment() = default;
    Comment(const QString& commentText,const QString& email) :
    _commentText(commentText),_email(email){}
    const QString &email() const;
    void setEmail(const QString &newEmail);
    const QString &commentText() const;
    void setCommentText(const QString &newCommentText);
};

class CommentsDataList : public QObject
{
    Q_OBJECT
private:
    QList<Comment> _comments;
public:
    explicit CommentsDataList(const QList<Comment>& comments,QObject *parent = nullptr);
    const QList<Comment> &comments() const;
    void setComments(const QList<Comment> &newComments);
    bool setItemAt(const int index,const Comment& video);
    static QList<Comment> createCommentsList(const QJsonDocument& document);
public slots:
    void addComment(const Comment& newVideo);
signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

};

