#include "plantmanager.h"

PlantManager::PlantManager(Plants *plants, QObject *parent) :
                            QObject(parent) , m_plants(plants)
{
    Q_ASSERT(plants != nullptr);
}

PlantEntry *PlantManager::createEntry()
{
    return m_plants->createPlant();
}

PlantEntry *PlantManager::addEntry(PlantEntry *entry)
{
    return m_plants->addPlant(entry);
}

bool PlantManager::removeEntry(PlantEntry *entry)
{
    return m_plants->removePlant(entry);
}

bool PlantManager::isEntryExists(PlantEntry *entry)
{
    return m_plants->findPlant(entry);
}

bool PlantManager::updateEntry(PlantEntry *entry)
{
    return m_plants->updatePlant(entry);
}

int PlantManager::cupNumber(PlantEntry *entry)
{
    return m_plants->getCupsNumber(entry);
}

Plants::PlantsList PlantManager::whichPlantsToWater(const int indexToday)
{
    return m_plants->toWater(indexToday);
}

QString PlantManager::getEntryName(PlantEntry *entry)
{
    return m_plants->getPlantName(entry);
}

PlantEntry *PlantManager::getElement(const int index)
{
    return m_plants->getElement(index);
}

int PlantManager::plantCount()
{
    return m_plants->getNumberofPlants();
}


