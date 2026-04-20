#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QVector>
#include <QUuid>
#include <QSettings>
#include "menuwidget.h"

class ApiClient : public QObject {
    Q_OBJECT
public:
    explicit ApiClient(QObject *parent = nullptr);

    void fetchScores();
    void postScore(const QString &name, int score);
    void checkNick(const QString &name);

signals:
    void scoresReceived(const QVector<ScoreEntry> &scores);
    void nickChecked(bool ok, const QString &reason);

private:
    QString getDeviceId();

    QNetworkAccessManager *manager = nullptr;
    QString baseUrl = QString("https://tetris-server-production-ed55.up.railway.app");
};

#endif
