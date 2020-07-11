#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QCoreApplication>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QWidget>
#include <QTime>
#include <QDate>
#include <QHash>
//#include <QCalendar>
#include <QTableWidget>
#include <QListWidget>
#include <QComboBox>
#include <QMessageBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QStringList>
#include <QSignalSpy>
#include <QPixmap>
#include <QFile>
#include <QIODevice>
#include <QtXml>
#include <QXmlStreamWriter>
#include <QStandardPaths>
#include <QDir>
#include <QAbstractItemView>
#include <fstream>

#include "plantmanager.h"
#include "plantentry.h"
#include "weather.h"
#include "keyboard.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(PlantManager *plantManager, QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void timerEvent(QTimerEvent *event);
private slots:

    void run_keyboard_lineEdit();
    void goToPlantManagement();
    void addPlant();
    void removePlant();
    void editPlant();
    void backToMainPage();
    void cancelChanges();
    void saveChanges();
    void getWeather();
    void setupWeather();

private:
    Ui::MainWindow *ui;
    Keyboard *lineEditkeyboard;
    PlantManager *m_plantManager;
    Weather *weatherManager;
    QGraphicsScene *m_scene;
    QFile m_dataFile;
    QDomDocument m_document;

    void setupSignals();
    void setupWidgets();
    void resetEdit();
    void initializePlantTable();
    void updatePlantTable();
    void readFromXml();
    void writeToXml();

    QHash<QListWidgetItem *,PlantEntry *> *m_entryMap;
};
#endif // MAINWINDOW_H
