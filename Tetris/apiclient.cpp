#include "apiclient.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QUrl>

ApiClient::ApiClient(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
}

void ApiClient::fetchScores()
{
    QNetworkRequest req(QUrl(baseUrl + "/scores"));
    QNetworkReply *reply = manager->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            reply->deleteLater();
            return;
        }
        QJsonArray arr = QJsonDocument::fromJson(reply->readAll()).array();
        QVector<ScoreEntry> scores;
        for (const QJsonValue &v : arr)
            scores.append({ v.toObject()["name"].toString(),
                           v.toObject()["score"].toInt() });
        emit scoresReceived(scores);
        reply->deleteLater();
    });
}

void ApiClient::checkNick(const QString &name)
{
    QNetworkRequest req(QUrl(baseUrl + "/check_nick"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject obj;
    obj["name"] = name;
    QNetworkReply *reply = manager->post(req, QJsonDocument(obj).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            emit nickChecked(true, ""); // при ошибке сети — пускаем
            reply->deleteLater();
            return;
        }
        QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();
        bool ok = obj["ok"].toBool();
        QString reason = obj["reason"].toString();
        emit nickChecked(ok, reason);
        reply->deleteLater();
    });
}

void ApiClient::postScore(const QString &name, int score)
{
    QNetworkRequest req(QUrl(baseUrl + "/scores"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject obj;
    obj["name"]  = name;
    obj["score"] = score;
    QNetworkReply *reply = manager->post(req, QJsonDocument(obj).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        fetchScores();
    });
}
