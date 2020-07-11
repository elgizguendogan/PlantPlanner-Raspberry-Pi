#ifndef PLANTS_H
#define PLANTS_H
#include "plantentry.h"
#include <QObject>

class Plants : public QObject
{
    Q_OBJECT
public:
    explicit Plants(QObject *parent = nullptr);

    typedef QList<PlantEntry *> PlantsList;

    PlantsList plantsList () const;

    PlantEntry *createPlant();
    PlantEntry *addPlant(PlantEntry *plant);

    bool removePlant(PlantEntry *plant);
    bool updatePlant(PlantEntry *plant);
    bool findPlant(PlantEntry *plant);

    int getCupsNumber(PlantEntry *plant);
    PlantsList toWater(const int dayOfWeek);
    QString getPlantName(PlantEntry *plant);
    int getNumberofPlants();
    PlantEntry *getElement(const int index);

signals:
    void entryAdded(PlantEntry *);
    void entryRemoved(PlantEntry *);
    void entryUpdated(PlantEntry *);
    void entryFound(PlantEntry *);
public slots:

private:
    PlantsList m_plantList;
};

#endif // PLANTS_H
