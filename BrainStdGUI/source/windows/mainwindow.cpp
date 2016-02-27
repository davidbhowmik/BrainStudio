#include "mainwindow.h"

MainWindow::MainWindow(QString filename, QWidget *parent) : QMainWindow(parent){
    setupUi(this);

    this->setWindowIcon(QIcon(":/new/prefix1/icons/brainstudio-logo.png"));

    welcomeWindow = NULL;
    aboutWindow = NULL;
    if(UserData::workspace_path == "empty" && filename==""){
        welcomeWindow = new WelcomeWindow();
        connect(welcomeWindow, SIGNAL(allDone()), this, SLOT(init()));
        welcomeWindow->show();
    }
    else
        this->init(filename);
}

MainWindow::~MainWindow() {
    /*if(welcomeWindow != NULL)
        delete welcomeWindow;*/
}

void MainWindow::init(QString givenfilewithpath=""){
    qDebug() << "MainWindow::init: workspace_path:" << UserData::workspace_path;

    /*if(welcomeWindow != NULL){
        delete welcomeWindow;
        welcomeWindow = NULL;
    }*/

    // Set the config.ini file -------------------------------------------------
    QString line, simulator = "tcpip";
    QFile file(UserData::workspace_path + "/config.ini");

    // Find the folder name and the timestepsize -------------------------------
    QRegExp *regExpSimulator = new QRegExp("\\{Simulator (.+)\\}");
    QRegExp *regExpTimeStepStize = new QRegExp("\\{timeStepSize (.+)\\}");
    int found = 0;
    dt = 0.25;
    std::cout << "FILE:" << QString(UserData::workspace_path + "/config.ini").toStdString() << std::endl;

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream inSim(&file);

        line = inSim.readLine();
        while(!line.isNull() && found < 2){
            if(regExpSimulator->indexIn(line) != -1){
                simulator = regExpSimulator->cap(1);
                found++;
            }
            if(regExpTimeStepStize->indexIn(line) != -1){
                dt = regExpTimeStepStize->cap(1).toDouble();
                found++;
            }
            line = inSim.readLine();
        }
        file.close();
    }
    delete regExpSimulator;
    delete regExpTimeStepStize;
    std::cout << "SIMULATOR:" << simulator.toStdString() << std::endl;

    // Find the folder name ----------------------------------------------------
    QRegExp *regExp = new QRegExp("\\{FolderName (.+)\\}");
    found = false;

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);

        line = in.readLine();
        while(!line.isNull() && !found){
            if(regExp->indexIn(line) != -1){
                FOLDERNAME = UserData::workspace_path + "/" + regExp->cap(1);
                found = true;
            }
            line = in.readLine();
        }
        file.close();
    }
    if(!found)
        FOLDERNAME = UserData::workspace_path;
    delete regExp;

    // -- GUI ------------------------------------------------------------------
    this->resize(1460, 980);
    this->move(0,0);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();

    this->tabWidget->setTabsClosable(true);
    this->saveButton->setEnabled(false);
    this->playButton->setEnabled(false);
    //this->learningCheckBox->setVisible(false);

    this->IPlineEdit->setText(UserData::tcpip_hostname);
    this->PORTlineEdit->setText(QString::number(UserData::tcpip_port));

    // -- VARIABLES ------------------------------------------------------------
    run = false;

    // -------------------------------------------------------------------------
    tcpip_init = new TCPIP_Init(this);
    //tcpip_init->connect_to_download_data();
    connect(tcpip_init, SIGNAL(nodes_downloaded(QString)),
            this, SLOT(nodes_downloaded(QString)));
    connect(tcpip_init, SIGNAL(edges_downloaded(QString)),
            this, SLOT(edges_downloaded(QString)));

    // To download backend data.
    this->on_PORTlineEdit_editingFinished();
    // -------------------------------------------------------------------------


    QFileInfo givenFile(givenfilewithpath);
    QFileInfo defaultFile(FOLDERNAME+"/schema1.brn");

    // If file is given try to load this file:
    if(givenFile.exists() && givenFile.isFile()){
        this->loadNewTab(givenfilewithpath);
    }
    // Else, try to load the default file
    else if(defaultFile.exists() && defaultFile.isFile()){
        this->loadNewTab("schema1.brn");
    }
    // If not show a new file
    else{
        this->onNew();
    }

    this->showMaximized();
}

bool MainWindow::noTabYet() const {
    if(tabWidget->currentIndex() < 0)
        return true;
    if(tab() == NULL)
        return true;
    return false;
}

void MainWindow::keyPressEvent(QKeyEvent * event){
    if(event->key()==Qt::Key_Escape)    onExit();
    else if(event->key()==Qt::Key_W)    onExit();
    else if(event->key()==Qt::Key_1)    selectTabWithKey(0);
    else if(event->key()==Qt::Key_2)    selectTabWithKey(1);
    else if(event->key()==Qt::Key_3)    selectTabWithKey(2);
    else if(event->key()==Qt::Key_4)    selectTabWithKey(3);
    else if(event->key()==Qt::Key_5)    selectTabWithKey(4);
    else if(event->key()==Qt::Key_6)    selectTabWithKey(5);
    else if(event->key()==Qt::Key_7)    selectTabWithKey(6);
    else if(event->key()==Qt::Key_8)    selectTabWithKey(7);
    else if(event->key()==Qt::Key_9)    selectTabWithKey(8);
    else if(event->key()==Qt::Key_F3)   { this->onActionsListToggle(); }
    else if(event->key()==Qt::Key_F4)   { this->onXmlToggle(); }
    else if(event->key()==Qt::Key_F6)   { this->onExperimentToggle(); }
    else if(event->key()==Qt::Key_F7)   { this->onPythonToggle(); }
    else if(event->key()==Qt::Key_Space)on_playButton_clicked();
    // MOVE BLOCKS WITH ARROWS
    else if(event->key()==Qt::Key_Up)   { if(!noTabYet()) tab()->keyUP(); }
    else if(event->key()==Qt::Key_Down) { if(!noTabYet()) tab()->keyDOWN(); }
    else if(event->key()==Qt::Key_Left) { if(!noTabYet()) tab()->keyLEFT(); }
    else if(event->key()==Qt::Key_Right){ if(!noTabYet()) tab()->keyRIGHT(); }
}


bool MainWindow::play(){
    if(!noTabYet() && tab()->play()){
        this->playButton->
                setIcon(QIcon(":/new/prefix1/icons/Pause-Disabled-icon48.png"));
        return true;
    }
    qDebug() << "MainWindow::play: failed.";
    return false;
}

bool MainWindow::pause(){
    if(!noTabYet() && tab()->pause()){
        playButton->setIcon(QIcon(":/new/prefix1/icons/Play-icon.png"));
        return true;
    }
    qDebug() << "MainWindow::pause: failed.";
    return false;
}

bool MainWindow::stop(){
    if(!noTabYet() && (this->pause() || true) && tab()->stop()){
        run = false;
        CreateNetworkButton->setIcon(QIcon(":/new/prefix1/icons/build.png"));
        IPlineEdit->setEnabled(true);
        PORTlineEdit->setEnabled(true);
        action_create_network->setText("Create network");
        playButton->setEnabled(false);
        return true;
    }
    qDebug() << "MainWindow::stop: failed.";
    return false;
}



// *******************************SLOTS************************************** //
void MainWindow::workTabSchemaLoaded(){
    qDebug() << "MainWindow::workTabSchemaLoaded: This function is empty!";
}

void MainWindow::workTabNetworkLoaded(){
    CreateNetworkButton->setIcon(QIcon(":/new/prefix1/icons/stop.png"));
    IPlineEdit->setEnabled(false);
    PORTlineEdit->setEnabled(false);
    action_create_network->setText("Stop simulation");
    playButton->setEnabled(true);
}

void MainWindow::workTabSchemaModifiedSlot(){
    this->saveButton->setEnabled(true);
}

void MainWindow::workTabGetTime(QString time){
    this->timeLabel->setText(time);
}

void MainWindow::workTabGetInfo(QList<float> data){
}
// -------------------------------------------------------------------------- //



// *****************************GUI SLOTS************************************ //
// ACTIONS
void MainWindow::on_action_create_network_triggered(){
    this->onCreateNetwork();
}
void MainWindow::on_action_new_schema_triggered(){
    this->onNew();
}
void MainWindow::on_action_load_schema_triggered(){
    this->onOpen();
}
void MainWindow::on_action_save_schema_triggered(){
    this->onSaveSchema();
}
void MainWindow::on_action_save_network_triggered(){
    this->onSaveNetwork();
}
void MainWindow::on_action_backup_schema_triggered(){
    this->onBackupSchema();
}
void MainWindow::on_action_exit_triggered(){
    this->onExit();
}
void MainWindow::on_action_about_triggered(){
    //QSound::play("hp.wav");
    aboutWindow = new AboutWindow(this);
    aboutWindow->show();
}
void MainWindow::on_action_grid_toggled(bool arg1){
    this->onSetGrid(arg1);
}
void MainWindow::on_actionXML_viewer_triggered(){
    this->onXmlToggle();
}
void MainWindow::on_xmlButton_clicked(){
    this->onXmlToggle();
}
void MainWindow::on_actionActions_list_triggered(){
    this->onActionsListToggle();
}
void MainWindow::on_actionsListButton_clicked(){
    this->onActionsListToggle();
}

void MainWindow::on_removeBlockButton_clicked(){
    if(!noTabYet() && tab()->removeBlock()){
        action_create_network->setEnabled(true);    // ????
        CreateNetworkButton->setEnabled(true);
    }
}
void MainWindow::on_IPlineEdit_editingFinished(){
    IPlineEdit->setStyleSheet("background: #fff;");
    PORTlineEdit->setStyleSheet("background: #fff;");
    tcpipLabel->setText("  Connecting..");
    UserData::tcpip_hostname = this->IPlineEdit->text();
    UserData::save();
    // -------------------------------------------------------------------------
    tcpip_init->connect_to_download_data();
}
void MainWindow::on_PORTlineEdit_editingFinished(){
    IPlineEdit->setStyleSheet("background: #fff;");
    PORTlineEdit->setStyleSheet("background: #fff;");
    tcpipLabel->setText("  Connecting..");
    UserData::tcpip_port = (int)this->PORTlineEdit->text().toShort();
    UserData::save();
    tcpip_init->connect_to_download_data();
}


// BUTTONS
void MainWindow::on_playButton_clicked(){
    this->onPlay();
}
void MainWindow::on_CreateNetworkButton_clicked(){
    this->onCreateNetwork();
}
void MainWindow::on_saveButton_clicked(){
    this->onSaveSchema();
}
void MainWindow::on_openFolderButton_clicked(){
    this->onOpenFolder();
}
// -------------------------------------------------------------------------- //

// ******************************METHODS************************************* //
void MainWindow::onExit(){
    //if(saveButton->isEnabled())
    //else
    //exit(1);
    this->close(); // So it can call the destructors!
}
void MainWindow::onNew(){
    QString filename = "unsaved"+QString::number(tabWidget->count()+1)+".brn";

    QFile data(FOLDERNAME+"/"+filename);
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
    QTextStream out(&data);
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"
               "<schema speed=\"1\">\n"
               "\t<actions>\n"
               "\t</actions>\n"
               "\t<nodes>\n"
               "\t</nodes>\n"
               "\t<edges>\n"
               "\t</edges>\n"
               "\t<textBlocks>\n"
               "\t</textBlocks>\n"
               "</schema>\n";
    }
    loadNewTab(filename);
}

void MainWindow::onOpen(){
    QString path =
                 QFileDialog::getOpenFileName(this,"Open a network",
                                              UserData::workspace_path,"*.brn");
    QStringList list1 = path.split("/", QString::SkipEmptyParts);
    QString filename;

    if(list1.size()>2){
        FOLDERNAME = path.left(path.size()-list1.last().size());
        filename  = list1[list1.size()-1];
    }
    else if(list1.size()>1){
        filename  = list1[list1.size()-1];
    }
    else{
        qDebug() << "MainWindow::open: Error: Path not recognized!";
    }

    if(UserData::workspace_path+"/" != FOLDERNAME){
        qDebug() << "MainWindow::onOpen: Path is different!";
        qDebug() << "The reason if it's not working could be the path."
                 << "\nPath: " << path
                 << "\nFOLDERNAME: " << FOLDERNAME
                 << "\nWorkspacePath: " << UserData::workspace_path
                 << "\nfilename: " << filename;
        // ZAF SOS: Display a message asking them if they want to change the
        //          workspace_path..
        loadNewTab(path);
    }
    else{
        loadNewTab(filename);
    }
}

bool MainWindow::onSaveSchema(){
    if(!noTabYet() && tab()->saveSchema() && tab()->loadXML()){
        this->saveButton->setEnabled(false);
        return true;
    }
    return false;
}

bool MainWindow::onSaveNetwork(){
    if(!noTabYet() && tab()->saveNetwork()){
        return true;
    }
    return false;
}

bool MainWindow::onBackupSchema(){
    if(!noTabYet() && tab()->backupSchema()){
        return true;
    }
    return false;
}
//        xdg-open /path/to/folde
bool MainWindow::onOpenFolder(){
    /*QString command; = QDir::currentPath();
    QString folder = FOLDERNAME;
    command.remove(QRegExp("build"));
    folder.remove(0,3);
    command += folder;*/

    QString command;
    #ifdef WIN32
        command = QDir::currentPath();
        QString folder = FOLDERNAME;
        command.remove(QRegExp("build"));
        folder.remove(0,3);
        command += folder;
        command.replace("/", "\\");
        qDebug() << "MainWindow::onOpenFolder: Opening " << command;
        command = "explorer.exe " + command;
    #elif linux
    command = "xdg-open " + UserData::workspace_path;
    qDebug() << "MainWindow::onOpenFolder: Opening " << command;
    #endif
    system(command.toStdString().c_str());

    return true;
}


bool MainWindow::onCreateNetwork(){
    if(!noTabYet()){
        if(!tab()->network_is_running())
            return tab()->create_network();
        else
            return this->stop();
    }
    return false;
}

void MainWindow::onPlay(){
    if(!noTabYet() && !tab()->network_is_running()){
        tab()->create_network();
        return; // returning because we need to wait for the network creation
    }

    if(!run) run = this->play();
    else run = !this->pause();
}

bool MainWindow::onSetGrid(const bool &on){
    if(noTabYet())
        return false;
    tab()->setGrid(on);
    return true;
}

void MainWindow::onXmlToggle(){
    if(noTabYet()){
        qDebug() << "MainWindow::onXmlToggle: Error: No tab yet!";
        return;
    }

    if(!tab()->xmlVisible()){
        xmlButton->setIcon(QIcon(":/new/prefix1/icons/xml-tool-icon.png"));
        actionXML_viewer->setChecked(true);
    }
    else{
        xmlButton->setIcon(QIcon(":/new/prefix1/icons/xml-tool-icon - gray.png"));
        actionXML_viewer->setChecked(false);
    }
    tab()->toggleXML();
}

void MainWindow::onActionsListToggle(){
    if(noTabYet()){
        qDebug() << "MainWindow::onActionsListToggle: Error: No tab yet!";
        return;
    }

    if(!tab()->actionsListVisible()){
        actionsListButton->setIcon(QIcon(":/new/prefix1/icons/"
                                         "actions-list.png"));
        actionActions_list->setChecked(true);
    }
    else{
        actionsListButton->setIcon(QIcon(":/new/prefix1/icons/"
                                         "actions-list-icon.png"));
        actionActions_list->setChecked(true);
    }
    tab()->toggleActionsList();
}

void MainWindow::onExperimentToggle(){
    if(noTabYet()){
        qDebug() << "MainWindow::onExperimentToggle: Error: No tab yet!";
        return;
    }

    /*if(!tab()->ExperimentVisible())
        ExperimentButton->setIcon(QIcon(":/new/prefix1/icons/"
                                        "actions-list.png"));
    else
        ExperimentButton->setIcon(QIcon(":/new/prefix1/icons/"
                                        "actions-list-icon.png"));*/
    tab()->toggleExperiment();
}

void MainWindow::onPythonToggle(){
    if(noTabYet()){
        qDebug() << "MainWindow::onPythonToggle: Error: No tab yet!";
        return;
    }

    /*if(!tab()->PythonVisible())
        pythonButton->setIcon(QIcon(":/new/prefix1/icons/"
                                    "actions-list.png"));
    else
        pythonButton->setIcon(QIcon(":/new/prefix1/icons/"
                                    "actions-list-icon.png"));*/
    tab()->togglePython();
}
// -------------------------------------------------------------------------- //






void MainWindow::on_rtNeuronSlider_sliderMoved(int position){
}


void MainWindow::nodes_downloaded(QString data){
    BackendData::load_nodes(data);
    tcpip_init->getEdges();
}


void MainWindow::edges_downloaded(QString data){
    BackendData::load_edges(data);
    IPlineEdit->setStyleSheet("background: #ccc;");
    PORTlineEdit->setStyleSheet("background: #ccc;");
    tcpipLabel->setText("  Connected!");

    tcpip_init->disconnect();
}


void MainWindow::error_workspaceTab_slot(QString msg){
    this->stop();
    QMessageBox msgBox;
    msgBox.critical(0,"Error", msg);
}











void MainWindow::on_actionRun_triggered(){
    this->play();
}

void MainWindow::on_actionPause_triggered(){
    this->pause();
}

void MainWindow::on_actionStop_Un_load_network_triggered(){
    this->stop();
}

void MainWindow::on_actionClear_screen_triggered(){
    if(!noTabYet()){
        tab()->removeAllBlocks();
    }
}








