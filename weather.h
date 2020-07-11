#ifndef WEATHER_H
#define WEATHER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QDebug>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QGraphicsView>
#include <QGraphicsScene>


class Weather : public QObject
{
    Q_OBJECT
public:
    explicit Weather(QObject *parent = nullptr);

    //void getLocationData();
    void getWeatherData(QString cityName);
    int convertToCels(double kelvin);

    QString getCityName() const;
    QString getCountryName() const;
    int getTemperature() const;
    QString getDescription() const;
    QPixmap getIcon() const;
    QString getIconName() const;

signals:
    void readyToGetWeatherInfo();
    void iconIdReady();
public slots:
    //void positionUpdated(const QGeoPositionInfo &info);
    void weatherDataFinish(QNetworkReply *reply);

private:
   QNetworkAccessManager m_manager;
   QString m_cityName;
   QString m_countryName;
   int m_temperature;
   QString m_description;
   QString m_iconName;
   QByteArray m_iconData;
   QPixmap m_icon;

   double m_lat;
   double m_lon;

};

#endif // WEATHER_H
