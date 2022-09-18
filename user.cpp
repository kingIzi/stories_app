#include <QUrl>
#include <QUrlQuery>
#include <QFile>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QMimeType>

#include "user.hpp"

User::Reply *User::firebaseSignUpReply(const QString email, const QString password)
{
    const auto signUpEndpoint = Endpoints::signUpWithEmailPassword;
    QUrl url(signUpEndpoint);
    QUrlQuery query;
    query.addQueryItem(Keys::key,ApiKeys::firebaseApi);
    url.setQuery(query);
    const QVariantMap signUpMap = {
        {QString(Keys::email),email},
        {QString(Keys::password),password},
        {QString(Keys::returnSecureToken),true}
    };
    const auto signUpDocument = QJsonDocument::fromVariant(signUpMap);
    User::Request request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    return this->_manager->post(request,signUpDocument.toJson());
}

User::Reply *User::makeMongoPostRequest(const QString &endpoint, const QJsonDocument &document)
{
    User::Request request((QUrl(endpoint)));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("Access-Control-Request-Headers","*");
    request.setRawHeader("api-key",ApiKeys::mongoDbApiKey);
    return this->_manager->post(request,document.toJson());
}

User::Reply *User::findOneById(const QString &id)
{
    const auto findDocument = this->getMongoDocument(this->filterById(id),MongoKeys::users,MongoKeys::filter);
    const auto findOneEndpoint = this->getMongoEndpoint(Endpoints::MongoEndpoints::findOne);
    return this->makeMongoPostRequest(findOneEndpoint,findDocument);
}

template <class T>
User::Reply *User::updateOne(const T &update, const QString &id, const QString &collection)
{
    const auto updateDocument = this->updateOneDocumentById(update,id,collection);
    const auto updateOneEndpoint = this->getMongoEndpoint(Endpoints::MongoEndpoints::updateOne);
    return this->makeMongoPostRequest(updateOneEndpoint,updateDocument);
}

User::Reply *User::addFileToStorage(const QUrl &url)
{
    auto urlString = url.toString();
    if (urlString.contains("file:///")){
        const auto string = urlString.remove(0,8);
        QFileInfo info(string);
        if (info.isFile()){
            const auto uploadUrl = QString(Endpoints::firebaseStorage) + QDate::currentDate().toString() + info.fileName();
            QNetworkRequest request(uploadUrl);
            QMimeDatabase db; const auto type = db.mimeTypeForFile(info.filePath());
            request.setHeader(QNetworkRequest::ContentTypeHeader,type.name());
            request.setRawHeader("Access-Control-Request-Headers","*");
            const auto bearer = "Bearer " + this->getIdToken();
            request.setRawHeader("Authorization",bearer.toStdString().c_str());
            QFile file(info.filePath());
            if (file.open(QIODevice::ReadOnly)){
                return this->_manager->post(request,file.readAll());
            }
            else { return nullptr; }
        }
        else { return nullptr; }
    }
    else { return nullptr; }
}

void User::connectSignUpReply(Reply *signUpReply,const QVariantMap &profile)
{
    QObject::connect(signUpReply,&User::Reply::readyRead,[signUpReply,profile,this](){
        const auto document = QJsonDocument::fromJson(signUpReply->readAll());
        if (document.object().contains(Keys::error)){
            const auto error = document.object().value(Keys::error).toObject();
            const auto message = error.value(Keys::message).toString();
            emit User::signUpFailed(message);
        }
        else if (document.object().contains(Keys::kind)){
            this->addUserProfile(document.object());
            const auto signUpProfile = this->getSignUpPayload(profile);
            const auto insertOneEndpoint = this->getMongoEndpoint(Endpoints::MongoEndpoints::insertOne);
            const auto insertUser = this->getMongoDocument(signUpProfile,MongoKeys::users,MongoKeys::document);
            const auto insertReply = this->makeMongoPostRequest(insertOneEndpoint,insertUser);
            this->connectInsertOneUser(insertReply);
        }
        signUpReply->deleteLater();
    });
}

void User::connectInsertOneUser(Reply *insertOneReply)
{
    QObject::connect(insertOneReply,&User::Reply::readyRead,[insertOneReply,this](){
        const auto document = QJsonDocument::fromJson(insertOneReply->readAll());
        const auto insertedId = document.object().value(Keys::insertedId).toString();
        const auto found = this->findOneById(insertedId);
        this->connectFoundUser(found);
        insertOneReply->deleteLater();
    });
}

void User::connectFoundUser(Reply *found)
{
    QObject::connect(found,&User::Reply::readyRead,[found,this](){
        const auto document = QJsonDocument::fromJson(found->readAll());
        const auto userObj = document.object().value(MongoKeys::document).toObject();
        this->addUserProfile(userObj);
        emit User::login();
        found->deleteLater();
    });
}

void User::connectSignInUser(Reply *signInReply)
{
    QObject::connect(signInReply,&User::Reply::readyRead,[this,signInReply](){
        const auto document = QJsonDocument::fromJson(signInReply->readAll());
        if (document.object().contains(Keys::error)){
            const auto error = document.object().value(Keys::error).toObject();
            const auto message = error.value(Keys::message).toString();
            qDebug() << message;
            emit User::signInFailed(message);
        }
        else if (document.object().contains(Keys::kind)){
            const auto object = document.object();
            this->addUserProfile(object);
            const QVariantMap emailFilter = {{QString(Keys::localId),this->getLocalId()}};
            const auto findOneDocument = this->getMongoDocument(emailFilter,MongoKeys::users,MongoKeys::filter);
            const auto findOneEndpoint = this->getMongoEndpoint(Endpoints::MongoEndpoints::findOne);
            const auto findReply = this->makeMongoPostRequest(findOneEndpoint,findOneDocument);
            this->connectFoundUser(findReply);
        }
        signInReply->deleteLater();
    });
}

void User::connectAllVideosReply(Reply *videosReply)
{
    QObject::connect(videosReply,&User::Reply::readyRead,[videosReply,this](){
        const auto documents = QJsonDocument::fromJson(videosReply->readAll());
        const auto videos = VideosDataList::createVideosList(documents);
        if (videos.size() > 0){
            const auto videosList = new VideosDataList(videos,this);
            emit User::allVideos(videosList);
        }
        videosReply->deleteLater();
    });
}

void User::connectVideoFoundReply(Reply *videoReply)
{
    QObject::connect(videoReply,&User::Reply::readyRead,[videoReply,this](){
        const auto json = QJsonDocument::fromJson(videoReply->readAll());
        const auto videoObj = json.object().value(MongoKeys::document).toObject();
        emit User::newVideo(videoObj.toVariantMap());
        videoReply->deleteLater();
    });
}

void User::connectObjectUpdated(Reply *updateReply,const QString& updatedId)
{
    QObject::connect(updateReply,&User::Reply::readyRead,[updateReply,updatedId,this](){
        const auto response = QJsonDocument::fromJson(updateReply->readAll());
        if (response.object().value("modifiedCount").toInt() > 0){
            emit User::objectUpdated(updatedId);
        }
        updateReply->deleteLater();
    });
}

void User::connectFileUploadReply(Reply *fileReply, const QString &id, const QString &collection, const QString &key)
{
    QObject::connect(fileReply,&User::Reply::readyRead,[key,id,fileReply,collection,this](){
        const auto document = QJsonDocument::fromJson(fileReply->readAll());
        const auto metadataMap = document.object().toVariantMap();
        const QVariantMap metadata = {
            {key,metadataMap}
        };
        const auto update = this->updateOne(metadata,id,collection);
        QObject::connect(this,&User::objectUpdated,this,&User::findUser);
        this->connectObjectUpdated(update,id);
        fileReply->deleteLater();
    });
    QObject::connect(fileReply,&User::Reply::uploadProgress,[=](qint64 uploaded,qint64 total){
        qDebug() << uploaded << total;
    });
}

void User::connectFavoriteVideosReply(Reply *favoriteReply)
{
    QObject::connect(favoriteReply,&User::Reply::readyRead,[favoriteReply,this](){
        const auto documents = QJsonDocument::fromJson(favoriteReply->readAll());
        const auto videos = VideosDataList::createFavoriteVideosList(documents,this->_favorites);
        if (videos.size() > 0){
            const auto favoritesList = new VideosDataList(videos,this);
            emit User::favoriteVideos(favoritesList);
        }
        favoriteReply->deleteLater();
    });
}

void User::connectAvailableVideosReply(Reply *videosReply)
{
    QObject::connect(videosReply,&User::Reply::readyRead,[videosReply,this](){
        const auto documents = QJsonDocument::fromJson(videosReply->readAll());
        const auto videosArray = documents.object().value(MongoKeys::documents).toArray();
        QList<Video> videos; videos.reserve(videosArray.size());
        std::for_each(videosArray.begin(),videosArray.end(),[&videos](const QJsonValue& value){
            const auto videoObj = value.toObject().toVariantMap();
            const auto video = VideosDataList::createVideo(videoObj);
            videos.push_back(video);
        });
        const auto availableList = new VideosDataList(videos,this);
        emit User::availableVideos(availableList);
        videosReply->deleteLater();
    });
}

void User::connectVideoCommentsReply(Reply *commentsReply)
{
    QObject::connect(commentsReply,&User::Reply::readyRead,[commentsReply,this](){
        const auto document = QJsonDocument::fromJson(commentsReply->readAll());
        const auto comments = CommentsDataList::createCommentsList(document);
        const auto commentsList = new CommentsDataList(comments,this);
        emit User::videoComments(commentsList);
        commentsReply->deleteLater();
    });
}

void User::connectRemoveVideoReply(Reply *reply)
{
    QObject::connect(reply,&User::Reply::readyRead,[reply,this](){
        const auto document = QJsonDocument::fromJson(reply->readAll());
        const auto count = document.object().value("deletedCount").toInt();
        if (count > 0) { this->getAllVideos(); }
        reply->deleteLater();
    });
}

void User::connectFindVideoForLikeReply(const QString& videoId,Reply *findVideoReply)
{
    QObject::connect(findVideoReply,&User::Reply::readyRead,[findVideoReply,videoId,this](){
        const auto json = QJsonDocument::fromJson(findVideoReply->readAll());
        const auto videoObj = json.object().value(MongoKeys::document).toObject();
        auto likes = videoObj.value(Keys::likes).toVariant().toStringList();
        this->likeVideo(videoId,likes);
        findVideoReply->deleteLater();
    });
}

void User::connectAddFavoriteVideo(Reply *favoriteReply)
{
    QObject::connect(favoriteReply,&User::Reply::readyRead,[favoriteReply,this](){
        const auto document = QJsonDocument::fromJson(favoriteReply->readAll());
        qDebug() << document;
        favoriteReply->deleteLater();
    });
}

void User::connectFindUserReply(Reply *findUserReply)
{
    QObject::connect(findUserReply,&User::Reply::readyRead,[findUserReply,this](){
        const auto document = QJsonDocument::fromJson(findUserReply->readAll());
        const auto userObj = document.object().value(MongoKeys::document).toObject();
        this->addUserProfile(userObj);
        findUserReply->deleteLater();
    });
}

void User::connectVideoForCommentReply(const QString &videoId, const QString& commentText,Reply *commentReply)
{
    QObject::connect(commentReply,&User::Reply::readyRead,[videoId,commentReply,commentText,this](){
        const auto document = QJsonDocument::fromJson(commentReply->readAll());
        const auto videoObj = document.object().value(MongoKeys::document).toObject();
        auto comments = videoObj.value(Keys::comments).toArray().toVariantList();
        this->appendUserComment(comments,commentText);
        const QVariantMap comment = {
            {QString(Keys::comments),comments}
        };
        QObject::connect(this,&User::objectUpdated,this,&User::findVideoComments);
        const auto updateOneReply = this->updateOne(comment,videoId,MongoKeys::videos);
        this->connectObjectUpdated(updateOneReply,videoId);
        commentReply->deleteLater();
    });
}

const QJsonDocument User::getMongoDocument(const QVariantMap &data, const QString &collection, const QString &key) const
{
    const QVariantMap document = {
        {QString(MongoKeys::dataSource),"Cluster0"},
        {QString(MongoKeys::database),"project_database"},
        {QString(MongoKeys::collection),collection},
        {key,data}
    };
    return QJsonDocument::fromVariant(document);
}

const QString User::getMongoEndpoint(const char *endpoint) const
{
    const QString baseUrl = Endpoints::MongoEndpoints::mongoDbBaseUrl;
    return baseUrl + endpoint;
}

const QVariantMap User::filterById(const QString &id)
{
    const QVariantMap filterObj = {
        {QString(MongoKeys::oid),id}
    };
    const QVariantMap filter = {
        {QString(MongoKeys::_id),filterObj}
    };
    return filter;
}

const QJsonDocument User::removeOneDocumentById(const QString &removeId, const QString collection)
{
    const auto id = this->filterById(removeId);
    const QVariantMap removeMap = {
        {QString(MongoKeys::dataSource),"Cluster0"},
        {QString(MongoKeys::database),"project_database"},
        {QString(MongoKeys::collection),collection},
        {QString(MongoKeys::filter),id},
    };
    return QJsonDocument::fromVariant(removeMap);
}

const QVariantMap User::getSignUpPayload(const QVariantMap& profile) const
{
    const QVariantMap signUp = {
        {QString(Keys::email),this->_profile.value(Keys::email).toString()},
        {QString(Keys::localId),this->_profile.value(Keys::localId).toString()},
        {QString(Keys::fullName),profile.value(Keys::fullName).toString()},
        {QString(Keys::telephone),profile.value(Keys::telephone).toString()},
        {QString(Keys::dateOfBirth),profile.value(Keys::dateOfBirth).toDate()}
    };
    return signUp;
}

template<class T>
const QJsonDocument User::updateOneDocumentById(const T &updateOne, const QString &updatingId, const QString collection)
{
    const auto id = this->filterById(updatingId);
    const QVariantMap update = {
        {QString(MongoKeys::set),updateOne}
    };
    const QVariantMap updateMap = {
        {QString(MongoKeys::dataSource),"Cluster0"},
        {QString(MongoKeys::database),"project_database"},
        {QString(MongoKeys::collection),collection},
        {QString(MongoKeys::filter),id},
        {QString(MongoKeys::update),update}
    };
    return QJsonDocument::fromVariant(updateMap);
}

void User::addUserProfile(const QJsonObject &profile)
{
    const auto keys = profile.keys();
    std::for_each(keys.begin(),keys.end(),[=](const QString& key){
        this->_profile.insert(key,profile.value(key));
    });
    if (keys.contains(Keys::favorites)){
        const auto favorites = this->_profile.value(Keys::favorites).toJsonValue().toVariant().toStringList();
        this->setFavorites(favorites);
    }
}

void User::appendUserComment(QVariantList &comments, const QString &commentText)
{
    QVariantMap comment;
    comment.insert(Keys::detailText,this->getEmail());
    comment.insert(Keys::text,commentText);
    comments.push_back(comment);
}

void User::likeVideo(const QString &videoId, QStringList &likes)
{
    if (likes.contains(this->getLocalId())){
        likes.removeOne(this->getLocalId());
    }
    else if (!likes.contains(this->getLocalId())){
        likes.push_back(this->getLocalId());
    }
    QVariantMap update; update.insert(Keys::likes,likes);
    QObject::connect(this,&User::objectUpdated,this,&User::findVideo);
    const auto updateOneReply = this->updateOne(update,videoId,MongoKeys::videos);
    this->connectObjectUpdated(updateOneReply,videoId);
}

void User::findVideo(const QString &videoId)
{
    const auto findOneDocument = this->getMongoDocument(this->filterById(videoId),MongoKeys::videos,MongoKeys::filter);
    const auto findOneEndpoint = this->getMongoEndpoint(Endpoints::MongoEndpoints::findOne);
    const auto findOneReply = this->makeMongoPostRequest(findOneEndpoint,findOneDocument);
    this->connectVideoFoundReply(findOneReply);
}

void User::findVideoComments(const QString &videoId)
{
    const auto findOneDocument = this->getMongoDocument(this->filterById(videoId),MongoKeys::videos,MongoKeys::filter);
    const auto findOneEndpoint = this->getMongoEndpoint(Endpoints::MongoEndpoints::findOne);
    const auto findOneReply = this->makeMongoPostRequest(findOneEndpoint,findOneDocument);
    this->connectVideoCommentsReply(findOneReply);
}

void User::findUser(const QString &userId)
{
    const auto findOneDocument = this->getMongoDocument(this->filterById(userId),MongoKeys::users,MongoKeys::filter);
    const auto findOneEndpoint = this->getMongoEndpoint(Endpoints::MongoEndpoints::findOne);
    const auto findOneReply = this->makeMongoPostRequest(findOneEndpoint,findOneDocument);
    this->connectFindUserReply(findOneReply);
}

void User::setFavorites(const QStringList &favorites)
{
    if (favorites != this->_favorites){
        this->_favorites = favorites;
        emit User::favoritesChanged();
    }
}

User::User(QObject *parent)
    : QObject{parent},
      _manager(std::make_unique<User::Manager>(this))
{

}

void User::signUpUser(QVariantMap profile)
{
    const auto emailPassword = [profile](){
        const auto email = profile.value(Keys::email).toString();
        const auto password = profile.value(Keys::password).toString();
        return std::make_pair(email,password);
    }();
    profile.remove(Keys::password);
    const auto signUpReply = this->firebaseSignUpReply(emailPassword.first,emailPassword.second);
    this->connectSignUpReply(signUpReply,profile);
}

void User::signInUser(const QString email, const QString password)
{
    const auto signUpEndpoint = Endpoints::signInWithEmailPassword;
    QUrl url(signUpEndpoint);
    QUrlQuery query;
    query.addQueryItem(Keys::key,ApiKeys::firebaseApi);
    url.setQuery(query);
    const QVariantMap signUpMap = {
        {QString(Keys::email),email},
        {QString(Keys::password),password},
        {QString(Keys::returnSecureToken),true}
    };
    const auto signUpDocument = QJsonDocument::fromVariant(signUpMap);
    User::Request request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    const auto signInReply = this->_manager->post(request,signUpDocument.toJson());
    this->connectSignInUser(signInReply);
}

const QString User::getEmail() const
{
    return this->_profile.value(Keys::email).toString();
}

const QString User::getUserId() const
{
    return this->_profile.value(MongoKeys::_id).toString();
}

const QString User::getIdToken() const
{
    return this->_profile.value(Keys::idToken).toString();
}

const QString User::getTelephone() const
{
    return this->_profile.value(Keys::telephone).toString();
}

const QString User::getFullName() const
{
    return this->_profile.value(Keys::fullName).toString();
}

const QString User::getDateOfBirth() const
{
    return this->_profile.value(Keys::dateOfBirth).toString();
}

const QString User::getUserType() const
{
    return this->_profile.value(Keys::userType).toString();
}

const QString User::getLocalId() const
{
    return this->_profile.value(Keys::localId).toString();
}

const QUrl User::getAvatar() const
{
    const auto avatar = this->_profile.value(Keys::avatar).toJsonValue().toObject().toVariantMap();
    if (avatar.isEmpty()) { return QUrl(); }
    Video video;
    return video.getUrlFromMetadata(avatar);
}

QStringList User::getFavoriteVideos()
{
    return this->_profile.value(Keys::favorites).toStringList();
}

void User::selectAllVideos()
{
    const auto multiple = this->getMongoDocument({},MongoKeys::videos,MongoKeys::filter);
    const auto findMultiEndpoint = this->getMongoEndpoint(Endpoints::MongoEndpoints::find);
    const auto multipleReply = this->makeMongoPostRequest(findMultiEndpoint,multiple);
    this->connectAllVideosReply(multipleReply);
}

void User::selectFavoriteVideos()
{
    const auto multiple = this->getMongoDocument({},MongoKeys::videos,MongoKeys::filter);
    const auto findMultiEndpoint = this->getMongoEndpoint(Endpoints::MongoEndpoints::find);
    const auto multipleReply = this->makeMongoPostRequest(findMultiEndpoint,multiple);
    this->connectFavoriteVideosReply(multipleReply);
}

void User::getAllVideos()
{
    const auto multiple = this->getMongoDocument({},MongoKeys::videos,MongoKeys::filter);
    const auto findMultiEndpoint = this->getMongoEndpoint(Endpoints::MongoEndpoints::find);
    const auto multipleReply = this->makeMongoPostRequest(findMultiEndpoint,multiple);
    this->connectAvailableVideosReply(multipleReply);
}

void User::watchVideo(const QString videoId,QVariantList watchedUsers)
{
    if (watchedUsers.contains(this->getLocalId())) { return; }
    watchedUsers.push_back(this->getLocalId());
    QVariantMap watched; watched.insert(Keys::watchedUsers,watchedUsers);
    QObject::connect(this,&User::objectUpdated,this,&User::findVideo);
    const auto updateOneReply = this->updateOne(watched,videoId,MongoKeys::videos);
    this->connectObjectUpdated(updateOneReply,videoId);
}

void User::likeVideo(const QString videoId)
{
    const auto findOneDocument = this->getMongoDocument(this->filterById(videoId),MongoKeys::videos,MongoKeys::filter);
    const auto findOneEndpoint = this->getMongoEndpoint(Endpoints::MongoEndpoints::findOne);
    const auto findOneReply = this->makeMongoPostRequest(findOneEndpoint,findOneDocument);
    this->connectFindVideoForLikeReply(videoId,findOneReply);
}

void User::addFavoriteVideo(const QString videoId)
{
    auto favorites = this->_favorites;
    if (favorites.contains(videoId)) {
        favorites.removeOne(videoId);
    }
    else if (!favorites.contains(videoId)){
        favorites.push_back(videoId);
    }
    this->setFavorites(favorites);
    QVariantMap favorite; favorite.insert(Keys::favorites,this->_favorites);
    QObject::connect(this,&User::objectUpdated,this,&User::findUser);
    const auto updateOneReply = this->updateOne(favorite,this->getUserId(),MongoKeys::users);
    this->connectAddFavoriteVideo(updateOneReply);
}

void User::updateUserProfile(QVariantMap profile)
{
    const auto avatar = profile.value(Keys::avatar).toUrl();
    profile.remove(Keys::avatar);
    QObject::connect(this,&User::objectUpdated,this,&User::findUser);
    this->updateOne(profile,this->getUserId(),MongoKeys::users);
    if (!avatar.isEmpty()){
        const auto avatarReply = this->addFileToStorage(avatar);
        this->connectFileUploadReply(avatarReply,this->getUserId(),MongoKeys::users,Keys::avatar);
    }
}

void User::addComment(const QString videoId,const QString commentText)
{
    if (!commentText.isEmpty()){
        const auto findOneDocument = this->getMongoDocument(this->filterById(videoId),MongoKeys::videos,MongoKeys::filter);
        const auto findOneEndpoint = this->getMongoEndpoint(Endpoints::MongoEndpoints::findOne);
        const auto findOneReply = this->makeMongoPostRequest(findOneEndpoint,findOneDocument);
        this->connectVideoForCommentReply(videoId,commentText,findOneReply);
    }
}

void User::removeVideo(const QString videoId)
{
    const auto removeEndpoint = this->getMongoEndpoint(Endpoints::MongoEndpoints::deleteOne);
    const auto removeDocument = this->removeOneDocumentById(videoId,MongoKeys::videos);
    const auto removeReply = this->makeMongoPostRequest(removeEndpoint,removeDocument);
    this->connectRemoveVideoReply(removeReply);
}

QStringList User::favorites()
{
    return this->_favorites;
}

