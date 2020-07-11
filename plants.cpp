#include "plants.h"

Plants::Plants(QObject *parent) : QObject(parent)
{

}

Plants::PlantsList Plants::plantsList() const
{
    return m_plantList;
}

PlantEntry *Plants::createPlant()
{
    PlantEntry* new_entry = new PlantEntry(this);
    if ( new_entry ) {
        m_plantList.append(new_entry);
        emit entryAdded(new_entry);
    }
    return new_entry;
}

PlantEntry *Plants::addPlant(PlantEntry *plant)
{
    if ( plant) {
        m_plantList.append(plant);
        emit entryAdded(plant);
    }
    return plant;
}


bool Plants::removePlant(PlantEntry *plant)
{
    if ( m_plantList.contains(plant)) {
        emit entryRemoved(plant);
        m_plantList.removeOne(plant);
        plant->deleteLater();
        return true;
    }

    return false;
}

bool Plants::updatePlant(PlantEntry *plant)
{
   if (findPlant(plant)) {
       int index = m_plantList.indexOf(plant);
       m_plantList.replace(index,plant);
       emit entryUpdated(plant);
       return true;
   }
   return false;
}

bool Plants::findPlant(PlantEntry *plant)
{
    if (m_plantList.contains(plant)) {
        emit entryFound(plant);
        return true;
    }
    return false;
}

int Plants::getCupsNumber(PlantEntry *plant)
{
    if ( findPlant(plant)) {
        return plant->cups();
    }
    else {
        return -1;
    }
}

Plants::PlantsList Plants::toWater(const int dayOfWeek)
{
    PlantsList plantsToWater;
    if ( dayOfWeek > 0 && dayOfWeek < 8) {
        for(auto& plantEntry : m_plantList){
            if ( plantEntry->isWaterDaySet(dayOfWeek-1)) {
                plantsToWater.append(plantEntry);
            }
        }
        return plantsToWater;
    }
    return {};
}

QString Plants::getPlantName(PlantEntry *plant)
{
    if ( findPlant(plant)) {
        return plant->name();
    }
    else {
        return {};
    }
}

int Plants::getNumberofPlants()
{
    return m_plantList.count();
}

PlantEntry *Plants::getElement(const int index)
{
    return m_plantList[index];
}
