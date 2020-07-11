#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(PlantManager *plantManager, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_plantManager(plantManager)
{
    ui->setupUi(this);
    this->resize(QDesktopWidget().availableGeometry(this).size() * 0.9);
    //int height = rec.height();
    //int width = rec.width();
    //this->setFixedSize(640,480);
    setWindowTitle(tr("Plant Planner"));
    this->setAttribute(Qt::WA_AcceptTouchEvents);

    lineEditkeyboard = new Keyboard();

    m_entryMap = new QHash<QListWidgetItem*, PlantEntry*>();
    ui->stackedWidget->setCurrentWidget(ui->mainPage);

    ui->getCityNameLabel->setText(QString("Enter city name for") + QString("\n" ) + QString("weather information"));
    //Create the scene and the gui, get the data and create the graphics view
    //Creating a timer for the weather function
    m_scene = new QGraphicsScene(this);
    startTimer(1000);
    setupWidgets();
    setupSignals();
    readFromXml();

    this->setStyleSheet("QMainWindow#MainWindow{background-image: url(:/images/backgroundImage2.png);background-repeat: no-repeat;} ");
    ui->stackedWidget->setCurrentWidget(ui->mainPage);

}

MainWindow::~MainWindow()
{
    writeToXml();
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(ui->stackedWidget->currentWidget() == ui->mainPage) {
        ui->time->setText(QDate::currentDate().toString("dd.MM.yyyy") + "\n" + QTime::currentTime().toString("hh:mm:ss"));
    }
}

/*void MainWindow::exitManual()
{
   this->close();
}*/

void MainWindow::setupSignals()
{
    connect(ui->managePlantsButton,&QPushButton::clicked, this, &MainWindow::goToPlantManagement);
    connect(ui->backMainButton, &QPushButton::clicked, this, &MainWindow::backToMainPage);
    connect(ui->addPlantButton, &QPushButton::clicked, this, &MainWindow::addPlant);
    connect(ui->removePlantButton, &QPushButton::clicked, this, &MainWindow::removePlant);
    connect(ui->editPlantButton, &QPushButton::clicked, this, &MainWindow::editPlant);
    connect(ui->backMainButton, &QPushButton::clicked, this, &MainWindow::backToMainPage);
    connect(ui->savePlantButton, &QPushButton::clicked, this, &MainWindow::saveChanges);
    connect(ui->cancelPlantButton, &QPushButton::clicked, this, &MainWindow::cancelChanges);
    connect(ui->getCityName,SIGNAL(selectionChanged()),this,SLOT(run_keyboard_lineEdit()));
    connect(ui->editName,SIGNAL(selectionChanged()),this,SLOT(run_keyboard_lineEdit()));
    connect(ui->getCityNamePushButton, &QPushButton::clicked, this, &MainWindow::getWeather)
;
}

void MainWindow::run_keyboard_lineEdit()
{
    QLineEdit *line = (QLineEdit *)sender();
    lineEditkeyboard->setLineEdit(line);
    lineEditkeyboard->show();
}

void MainWindow::setupWidgets()
{
    //Setup main table
    initializePlantTable();

    //Setup days to water list
    QList<QString> days;
    days << "Monday" << "Tuesday" << "Wednesday" << "Thursday" << "Friday" << "Saturday" << "Sunday";

    for ( int i=0; i < 7;++i) {
        QListWidgetItem* newItem = new QListWidgetItem(days[i], ui->dayList);
        newItem->setFlags(Qt::ItemIsUserCheckable | newItem->flags());
        newItem->setCheckState(Qt::Unchecked);
        ui->dayList->addItem(newItem);
    }
    //Setup combo box
    ui->cupsNumberComboBox->addItem(tr("-Select-"));
    for ( int i=1; i < 21; ++i) {
        ui->cupsNumberComboBox->addItem(QString::number(i) + " cups");
    }

    ui->editPlantButton->setDisabled(true);
    ui->removePlantButton->setDisabled(true);

}

void MainWindow::getWeather()
{
    QString cityName = ui->getCityName->text();
    weatherManager = new Weather;
    weatherManager->getWeatherData(cityName);
    connect(weatherManager,SIGNAL(iconIdReady()),this,SLOT(setupWeather()));
}

void MainWindow::setupWeather()
{
    ui->showTemprature->setText(QString::number(weatherManager->getTemperature())+" °C");

    QPixmap iconPixmap;
    iconPixmap.load(":/weatherIcons/icons/"+weatherManager->getIconName()+".png",nullptr, Qt::ImageConversionFlags());
    ui->showIcon->setPixmap(iconPixmap);

    ui->showCityName->setText(weatherManager->getCityName()+","+weatherManager->getCountryName());
}

void MainWindow::resetEdit()
{
    ui->editName->setText(tr(""));
    for ( int i=0; i < 7;++i) {
       ui->dayList->item(i)->setCheckState(Qt::Unchecked);
     }
    ui->cupsNumberComboBox->setCurrentIndex(0);
}

void MainWindow::initializePlantTable()
{
    ui->plantTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->plantTable->verticalHeader()->setVisible(false);
    ui->plantTable->setRowCount(0);
    ui->plantTable->setColumnCount(2);
    ui->plantTable->setColumnWidth(0,ui->plantTable->width()/2);
    ui->plantTable->setColumnWidth(1,ui->plantTable->width()/3);
    //ui->plantTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QStringList tableHeader;
    tableHeader<<"Plants"<<"Cups";
    ui->plantTable->setHorizontalHeaderLabels(tableHeader);
    QAbstractItemView::EditTriggers trigger = {QAbstractItemView::NoEditTriggers};
    ui->plantTable->setEditTriggers(trigger);
}

void MainWindow::updatePlantTable()
{
    QDate dateToday = QDate::currentDate();
    Plants::PlantsList plantsToWater = m_plantManager->whichPlantsToWater(dateToday.dayOfWeek());
    ui->plantTable->setRowCount(0);

    if ( !plantsToWater.empty()) {
        int rowSize = plantsToWater.count();

        ui->plantTable->setRowCount(rowSize);
        ui->plantTable->setColumnCount(2);

        for ( int i = 0; i < rowSize ; ++i ) {
            if ( !m_plantManager->getEntryName(plantsToWater[i]).isEmpty() && m_plantManager->cupNumber(plantsToWater[i]) != -1) {
                QTableWidgetItem *itemName = new QTableWidgetItem(m_plantManager->getEntryName(plantsToWater[i]),0);
                ui->plantTable->setItem(i,0, itemName);

                QTableWidgetItem *itemCups = new QTableWidgetItem(QString::number(m_plantManager->cupNumber(plantsToWater[i])),0);
                ui->plantTable->setItem(i,1, itemCups);
             }
        }
    }
}

void MainWindow::goToPlantManagement()
{
    ui->stackedWidget->setCurrentWidget(ui->plantManagePage);
}

void MainWindow::addPlant()
{
    //Reset the plant editing screen
    resetEdit();
    ui->stackedWidget->setCurrentWidget(ui->plantEditPage);
}

void MainWindow::removePlant()
{
    auto item = ui->plantList->currentItem();
    auto entry = m_entryMap->value(item);

    if ( item && entry) {
        if ( m_plantManager->removeEntry(entry) ) {
            m_entryMap->remove(item);
            delete item;
        }
    }
    if ( m_entryMap->isEmpty()) {
        ui->editPlantButton->setDisabled(true);
        ui->removePlantButton->setDisabled(true);
    }
    updatePlantTable();
}

void MainWindow::editPlant()
{
     auto item = ui->plantList->currentItem();
     auto entry = m_entryMap->value(item);
     // Fill the form with the information of the selected plant
     if ( item && entry) {
         ui->editName->setText(entry->name());
         for ( int i=0; i < 7;++i) {
            if (entry->isWaterDaySet(i)) {
                ui->dayList->item(i)->setCheckState(Qt::Checked);
            }
            else {
                ui->dayList->item(i)->setCheckState(Qt::Unchecked);
            }
         }
         ui->cupsNumberComboBox->setCurrentIndex(entry->cups());
     }

     ui->stackedWidget->setCurrentWidget(ui->plantEditPage);
}

void MainWindow::backToMainPage()
{
    ui->stackedWidget->setCurrentWidget(ui->mainPage);
}

void MainWindow::cancelChanges()
{
    resetEdit();
    ui->stackedWidget->setCurrentWidget(ui->plantManagePage);
}

void MainWindow::saveChanges()
{
    bool isInfoComplete = true;
    QMessageBox msgBox;
    PlantEntry *newEntry = new PlantEntry(this);

    // Set the Name
    if ( !ui->editName->text().isEmpty() ) {
        newEntry->setName(ui->editName->text());
    }
    else {
        //Give Warning
        msgBox.setText(tr("Please give your plant a name"));
        msgBox.exec();
        isInfoComplete = false;
    }
    // Set the Watering Days
    QBitArray waterDays(7);
    for ( int i=0; i < 7;++i) {
       if(ui->dayList->item(i)->checkState()) {
           waterDays[i] = true;
       }
       else {
           waterDays[i] = false;
       }
    }
    newEntry->setWaterDays(waterDays);

    //Set the cups number
    if (ui->cupsNumberComboBox->currentIndex() != 0) {
        newEntry->setCups(ui->cupsNumberComboBox->currentIndex());
    }
    else {
        //Give Warning
        msgBox.setText(tr("Please select a cup number"));
        msgBox.exec();
        isInfoComplete = false;
    }


    if(isInfoComplete) {
        QList<QListWidgetItem *> items = ui->plantList->findItems(newEntry->name(),Qt::MatchExactly);
        if ( items.count() > 0 ){
           // There is already an element update it
            m_entryMap->insert(items[0],newEntry);
            //m_entryMap[items[0]] = newEntry;
            m_plantManager->updateEntry(newEntry);
        }
        else {
            m_plantManager->addEntry(newEntry);
            ui->plantList->addItem( newEntry->name() );
            auto item = ui->plantList->item( ui->plantList->count() - 1 );
            m_entryMap->insert(item, newEntry);
            writeToXml();
        }

        updatePlantTable();
        ui->editPlantButton->setDisabled(false);
        ui->removePlantButton->setDisabled(false);
        ui->stackedWidget->setCurrentWidget(ui->plantManagePage);
        resetEdit();
    }
}

void MainWindow::readFromXml()
{

    QString appPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QDir dir(appPath);
    if (!dir.exists()) {
        dir.mkpath(appPath);
    }

    //QString filePath = QDir::currentPath();
    QString filePath = appPath.append("/plantData.xml");
    //QCoreApplication::applicationDirPath();

    //filePath.append("/plantData.xml");

    QFile dataFile(filePath);
    QDomDocument domDoc;
    QIODevice::OpenMode mode = {QIODevice::ReadOnly | QIODevice::Text };

    if(!dataFile.open(mode))
    {
        //qDebug() << "Failed to open file";

    }
    else
    {
        if (!domDoc.setContent(&dataFile))
        {
            //qDebug() << "Failed to load document";

        }
        dataFile.close();
    }

    QDomElement root = domDoc.firstChildElement();
    QDomNodeList nodeList = root.elementsByTagName("Plant");
    int listCount = nodeList.count();

    for (int i=0; i < listCount; ++i) {
        QDomNode plantNode = nodeList.at(i);
        if (plantNode.isElement()) {
            QDomElement plantElem = plantNode.toElement();
            PlantEntry *newEntry = new PlantEntry(this);;
            newEntry->setName(plantElem.attribute("Name"));
            newEntry->setCups(plantElem.attribute("Cups").toInt());
            QBitArray waterDays(7);
            waterDays[0] = (plantElem.attribute("Monday") == "1" ? true : false);
            waterDays[1] = (plantElem.attribute("Tuesday") == "1" ? true : false);
            waterDays[2] = (plantElem.attribute("Wednesday") == "1" ? true : false);
            waterDays[3] = (plantElem.attribute("Thursday") == "1" ? true : false);
            waterDays[4] = (plantElem.attribute("Friday") == "1" ? true : false);
            waterDays[5] = (plantElem.attribute("Saturday") == "1" ? true : false);
            waterDays[6] = (plantElem.attribute("Sunday") == "1" ? true : false);
            newEntry->setWaterDays(waterDays);
            m_plantManager->addEntry(newEntry);
            ui->plantList->addItem( newEntry->name() );
            auto item = ui->plantList->item( ui->plantList->count() - 1 );
            m_entryMap->insert(item, newEntry);
        }
    }

    if (listCount > 0) {
        updatePlantTable();
        ui->editPlantButton->setDisabled(false);
        ui->removePlantButton->setDisabled(false);
    }


}

void MainWindow::writeToXml()
{
    QDomDocument domDoc;
    QDomElement root = domDoc.createElement("Plants");
    domDoc.appendChild(root);

    int plantCount =  m_plantManager->plantCount();
    for (int i = 0; i < plantCount; ++i) {
        PlantEntry *tempElem = m_plantManager->getElement(i);
        QDomElement xmlElem = m_document.createElement("Plant");
        xmlElem.setAttribute("Name", tempElem->name());
        xmlElem.setAttribute("Cups", tempElem->cups());
        xmlElem.setAttribute("Monday", tempElem->isWaterDaySet(0));
        xmlElem.setAttribute("Tuesday", tempElem->isWaterDaySet(1));
        xmlElem.setAttribute("Wednesday", tempElem->isWaterDaySet(2));
        xmlElem.setAttribute("Thursday", tempElem->isWaterDaySet(3));
        xmlElem.setAttribute("Friday", tempElem->isWaterDaySet(4));
        xmlElem.setAttribute("Saturday", tempElem->isWaterDaySet(5));
        xmlElem.setAttribute("Sunday", tempElem->isWaterDaySet(6));
        root.appendChild(xmlElem);

    }
    //QString filePath = QDir::currentPath();
    //QCoreApplication::applicationDirPath();

    //filePath.append("/plantData.xml");

    QString appPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    //QString filePath = QDir::currentPath();
    QString filePath = appPath.append("/plantData.xml");

    QFile dataFile(filePath);

    QIODevice::OpenMode mode = {QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text };
    if(!dataFile.open(mode))
    {
        //qDebug() << "Failed to open file for writting";
    }
    else
    {
        QTextStream stream(&dataFile);
        stream << domDoc.toString();
        dataFile.close();
        //qDebug() << "Finished";
    }

}

