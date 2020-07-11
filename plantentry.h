#ifndef PLANTENTRY_H
#define PLANTENTRY_H

#include <QObject>
#include <QString>
#include <QBitArray>

class PlantEntry : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged);
    Q_PROPERTY(QBitArray waterDays READ waterDays WRITE setWaterDays NOTIFY waterDaysChanged);
public:
    explicit PlantEntry(QObject *parent);

    QString name() const;
    void setName(const QString &name);

    QBitArray waterDays() const;
    void setWaterDays(const QBitArray &waterDays);

    bool isWaterDaySet(const int index);

    int cups() const;
    void setCups(int cups);

signals:
    void nameChanged();
    void waterDaysChanged();

public slots:

private:
    QString m_name;
    QBitArray m_waterDays;
    int m_cups;

};

#endif // PLANTENTRY_H
