#ifndef PLANTMANAGER_H
#define PLANTMANAGER_H
#include "plants.h"
#include <QObject>

class PlantManager : public QObject
{
    Q_OBJECT
public:
    explicit PlantManager(Plants *plants, QObject *parent = nullptr);

    PlantEntry *createEntry();
    PlantEntry *addEntry(PlantEntry *entry);

    bool removeEntry(PlantEntry *entry);
    bool isEntryExists(PlantEntry *entry);
    bool updateEntry(PlantEntry *entry);

    int cupNumber(PlantEntry *entry);
    Plants::PlantsList whichPlantsToWater(const int indexToday);
    QString getEntryName(PlantEntry *entry);
    PlantEntry *getElement(const int index);
    int plantCount();

signals:

public slots:

private:
    Plants *m_plants;
};

#endif // PLANTMANAGER_H
