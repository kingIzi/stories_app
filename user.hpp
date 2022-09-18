#pragma once

//C++ Standard Includes
#include <memory>
#include <functional>

//QT Includes
#include <QObject>

#include <QString>
#include <QStringList>

#include <QVariantMap>
#include <QVariant>
#include <QVariantList>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "videoslistmodel.hpp"
#include "commentslistmodel.hpp"

struct ApiKeys{
    ApiKeys() = delete;
    static constexpr const char* firebaseApi = "AIzaSyAi9lxhaqrWZEyJtLkA9Lce1pa_3GfJq1g";
    static constexpr const char* mongoDbApiKey = "do6C4yFQHuhZVkEmFXJQulHG2vugYt1DLJFQ2bqQSMjo8A0Bz0TmjjbERC81FwfL";
};

struct Endpoints{
    Endpoints() = delete;
    static constexpr const char* signUpWithEmailPassword = "https://identitytoolkit.googleapis.com/v1/accounts:signUp";
    static constexpr const char* signInWithEmailPassword = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword";
    static constexpr const char* firebaseStorage = "https://firebasestorage.googleapis.com/v0/b/kongo-history.appspot.com/o/";

    struct MongoEndpoints{
        MongoEndpoints() = delete;
        static constexpr const char* mongoDbBaseUrl = "https://data.mongodb-api.com/app/data-gxvdg/endpoint/data/v1/";
        static constexpr const char* insertOne = "action/insertOne";
        static constexpr const char* findOne = "action/findOne";
        static constexpr const char* find = "action/find";
        static constexpr const char* updateOne = "action/updateOne";
        static constexpr const char* deleteOne = "action/deleteOne";
    };
};

struct MongoKeys{
    MongoKeys() = delete;
    static constexpr const char* dataSource = "dataSource";
    static constexpr const char* database = "database";
    static constexpr const char* collection = "collection";
    static constexpr const char* filter = "filter";
    static constexpr const char* document = "document";
    static constexpr const char* documents = "documents";
    static constexpr const char* users = "users";
    static constexpr const char* videos = "videos";
    static constexpr const char* oid = "$oid";
    static constexpr const char* _id = "_id";
    static constexpr const char* set = "$set";
    static constexpr const char* update = "update";
};

struct Keys{
    static constexpr const char* key = "key";
    static constexpr const char* email = "email";
    static constexpr const char* password = "password";
    static constexpr const char* returnSecureToken = "returnSecureToken";
    static constexpr const char* idToken = "idToken";
    static constexpr const char* localId = "localId";
    static constexpr const char* telephone = "telephone";
    static constexpr const char* fullName = "fullName";
    static constexpr const char* dateOfBirth = "dateOfBirth";
    static constexpr const char* error = "error";
    static constexpr const char* kind = "kind";
    static constexpr const char* message = "message";
    static constexpr const char* insertedId = "insertedId";
    static constexpr const char* watchedUsers = "watchedUsers";
    static constexpr const char* likes = "likes";
    static constexpr const char* favorites = "favorites";
    static constexpr const char* avatar = "avatar";
    static constexpr const char* comments = "comments";
    static constexpr const char* comment = "comment";
    static constexpr const char* detailText = "detailText";
    static constexpr const char* text = "text";
    static constexpr const char* userType = "userType";
};

class User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList favorites READ favorites WRITE setFavorites NOTIFY favoritesChanged);
private:
    using Manager = QNetworkAccessManager;
    using Reply = QNetworkReply;
    using Request = QNetworkRequest;
    using emitVideosDataList = std::function<void(VideosDataList*)>;
  private:
    QVariantMap _profile = {};
    QStringList _favorites;
    std::unique_ptr<User::Manager> _manager = nullptr;
    std::unique_ptr<CommentsDataList> _currentVideoComments = nullptr;
private: //RESTFUL REPLIES
    User::Reply* firebaseSignUpReply(const QString email,const QString password);
    User::Reply* makeMongoPostRequest(const QString &endpoint, const QJsonDocument &document);
    User::Reply* findOneById(const QString& id);
    template <class T>
    User::Reply* updateOne(const T& update,const QString& id,const QString& collection);
    User::Reply* addFileToStorage(const QUrl &url);
private: //NETWORK REPLY CONNECTIONS
    void connectSignUpReply(User::Reply* signUpReply,const QVariantMap& profile);
    void connectInsertOneUser(User::Reply* insertOneReply);
    void connectFoundUser(User::Reply* found);
    void connectSignInUser(User::Reply* signInReply);
    void connectAllVideosReply(User::Reply* videosReply);
    void connectVideoFoundReply(User::Reply* videoReply);
    void connectObjectUpdated(User::Reply* updateReply,const QString& updatedId);
    void connectFileUploadReply(User::Reply* fileReply,const QString& id,const QString& collection,const QString& key);
    void connectFavoriteVideosReply(User::Reply* favoriteReply);
    void connectAvailableVideosReply(User::Reply* videosReply);
    void connectVideoCommentsReply(User::Reply* commentsReply);
    void connectRemoveVideoReply(User::Reply* reply);
    void connectFindVideoForLikeReply(const QString& videoId,User::Reply* findVideoReply);
    void connectAddFavoriteVideo(User::Reply* favoriteReply);
    void connectFindUserReply(User::Reply* findUserReply);
    void connectVideoForCommentReply(const QString& videoId,const QString& commentText,User::Reply* commentReply);
private: //DOCUMENT PAYLOADS
    const QJsonDocument getMongoDocument(const QVariantMap& data,const QString& collection,const QString& key) const;
    const QString getMongoEndpoint(const char* endpoint) const;
    const QVariantMap filterById(const QString& id);
    template <class T>
    const QJsonDocument updateOneDocumentById(const T &updateOne, const QString& updatingId,const QString collection);
    const QJsonDocument removeOneDocumentById(const QString& removeId,const QString collection);
    const QVariantMap getSignUpPayload(const QVariantMap& profile) const;
private: //HELPER METHODS
    void addUserProfile(const QJsonObject& profile);
    void appendUserComment(QVariantList& comments,const QString& commentText);
    void likeVideo(const QString& videoId,QStringList& likes);
private slots:
    void findUser(const QString& userId);
    void setFavorites(const QStringList& favorites);
public slots:
    void findVideo(const QString& videoId);
    void findVideoComments(const QString& videoId);
public:
    explicit User(QObject *parent = nullptr);
    Q_INVOKABLE void signUpUser(QVariantMap profile);
    Q_INVOKABLE void signInUser(const QString email,const QString password);
    Q_INVOKABLE const QString getEmail() const;
    Q_INVOKABLE const QString getUserId() const;
    Q_INVOKABLE const QString getIdToken() const;
    Q_INVOKABLE const QString getTelephone() const;
    Q_INVOKABLE const QString getFullName() const;
    Q_INVOKABLE const QString getDateOfBirth() const;
    Q_INVOKABLE const QString getUserType() const;
    Q_INVOKABLE const QString getLocalId() const;
    Q_INVOKABLE const QUrl getAvatar() const;
    Q_INVOKABLE QStringList getFavoriteVideos();
    Q_INVOKABLE void selectAllVideos();
    Q_INVOKABLE void selectFavoriteVideos();
    Q_INVOKABLE void getAllVideos();
    Q_INVOKABLE void watchVideo(const QString videoId,QVariantList watchedUsers);
    Q_INVOKABLE void likeVideo(const QString videoId);
    Q_INVOKABLE void addFavoriteVideo(const QString videoId);
    Q_INVOKABLE void updateUserProfile(QVariantMap profile);
    Q_INVOKABLE void addComment(const QString videoId,const QString commentText);
    Q_INVOKABLE void removeVideo(const QString videoId);
    QStringList favorites();
signals:
    void signUpFailed(const QString);
    void signInFailed(const QString);
    void login();
    void allVideos(VideosDataList*);
    void newVideo(QVariantMap);
    void objectUpdated(const QString);
    void favoriteVideos(VideosDataList*);
    void favoritesChanged();
    void availableVideos(VideosDataList*);
    void videoComments(CommentsDataList*);
};

