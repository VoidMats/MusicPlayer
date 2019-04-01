#include "mediaplayer.h"
#include "ui_mediaplayer.h"

MediaPlayer::MediaPlayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MediaPlayer)
{
    ui->setupUi(this);

    // Media classes
    pPlayer = new QMediaPlayer(this);
    pPlayList = new QMediaPlaylist(this);

    // Other classes
    pListModel = new QStringListModel(this);

    // SIGNAL & SLOT assignment
    //connect(pPlayer, &QMediaPlayer::positionChanged, this, &MediaPlayer::o)
    //connect(pPlayList, QMediaPlaylist::loaded, this, &MediaPlayer::on_loaded);
    connect(pPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(on_state_changed(QMediaPlayer::State)));
    connect(ui->hzVolume, SIGNAL(valueChanged(int)), this, SLOT(on_hzVolume_valueChanged(int)));

    // Setup MediaPlayer
    ui->hzVolume->setValue(50);
    pPlayer->setPlaylist(pPlayList);
    pListModel->setStringList(listTunes);
    ui->lTunes->setModel(pListModel);
}

MediaPlayer::~MediaPlayer()
{
    delete pPlayer;
    delete pPlayList;
    delete pListModel;
    delete ui;
}

void MediaPlayer::on_actionAdd_files_triggered()
{
    QFileDialog dlg;
    dlg.setFileMode(QFileDialog::ExistingFiles);
    //dlg.setNameFilter(tr("Music file (*.mp3)");
    dlg.setOption(QFileDialog::ReadOnly);
    dlg.setViewMode((QFileDialog::List));
    QList<QUrl> listFileUrl = dlg.getOpenFileUrls(this,
                                                  tr("Open music file"),
                                                  QDir::currentPath(),
                                                  tr("Music (*.mp3)"));
    if( !listFileUrl.isEmpty() ) {
        QString sError{""};
        setFilesToPlayer(listFileUrl, sError);
        if( !sError.isEmpty() ) {
            statusBar()->clearMessage();
            statusBar()->showMessage(sError);
        }
    }
}

void MediaPlayer::on_actionClear_list_triggered()
{
    pPlayList->clear();
    if(pListModel->rowCount() != 0) {
        bool errorRemove = pListModel->removeRows(0,pListModel->rowCount());
        if(!errorRemove)
            qDebug() << "Fail to remove rows in model" << endl;
        ui->lTunes->update();
    }
    setTextStatusbar();
}

void MediaPlayer::on_actionSave_list_triggered()
{
    QFileDialog dlg;
    dlg.setFileMode(QFileDialog::AnyFile);
    QUrl saveFileName = dlg.getSaveFileUrl(this,
                                       tr("Save playlist"),
                                       QDir::currentPath(),
                                       tr("Playlist file (*.m3u)"));

    if(!saveFileName.isEmpty()) {
        // TODO check if the file extension exist
        pPlayList->save(saveFileName, "m3u");
    }
}

void MediaPlayer::on_actionload_list_triggered()
{
    QFileDialog dlg;
    dlg.setFileMode(QFileDialog::ExistingFile);
    QUrl loadFileName = dlg.getOpenFileUrl(this,
                                           tr("Load playlist"),
                                           QDir::currentPath(),
                                           tr("Playlist (*.m3u"));
    qDebug() << "Loaded filename: " << loadFileName;
    if(!loadFileName.isEmpty()) {
        pPlayList->load(loadFileName, "m3u");
    }

}

void MediaPlayer::on_actionExit_triggered()
{
   pPlayer->stop();
   qApp->exit();
}

void MediaPlayer::setFilesToPlayer(QList<QUrl> _listFiles, QString &_error)
{
    bool bError{false};
    for ( auto it : _listFiles ) {
        bError = pPlayList->addMedia(it);
        if ( !bError ) {
            // TODO add exception
            _error = pPlayList->errorString();
            break;
        }
        else {
            QVariant fileName{it.fileName()};
            setFilesToModel(fileName, _error);
        }
    }
}

void MediaPlayer::setFilesToModel(QVariant _fileName, QString &_error)
{
    // Create row in model
    bool errorInsert = pListModel->insertRow(pListModel->rowCount());
    // Get last index from inserted row
    QModelIndex idxFile{pListModel->index(pListModel->rowCount()-1,0)};
    // Populate row with data using index
    bool errorSetData = pListModel->setData(idxFile, _fileName);
    if( !errorInsert || !errorSetData ) {
        QString temp{"Error while inserting row in model: " + QString::number(errorInsert)};
        temp.append(". Error while insert data into row: " + QString::number(errorSetData));
        _error = temp;
    }
}

void MediaPlayer::setTextStatusbar()
{
    statusBar()->clearMessage();
    statusBar()->showMessage(pPlayList->currentMedia().canonicalUrl().fileName());
}

void MediaPlayer::on_pbPlay_clicked()
{
    QMediaPlayer::State state{pPlayer->state()};
    switch (state) {
    case QMediaPlayer::StoppedState:
        pPlayer->play();
        qDebug() << "Music playing: " << pPlayList->currentMedia().canonicalUrl().fileName();
        setTextStatusbar();
        break;
    case QMediaPlayer::PausedState:
        break;
    case QMediaPlayer::PlayingState:
        ui->pbPlay->setDown(true);
        break;
    }
}

void MediaPlayer::on_pbStop_clicked()
{
    pPlayer->stop();
    qDebug() << "Player stopped" << endl;
}

void MediaPlayer::on_state_changed(QMediaPlayer::State _state)
{
    switch(_state) {
    case QMediaPlayer::StoppedState:
        ui->pbPlay->setDown(false);
        break;
    case QMediaPlayer::PausedState:
        break;
    case QMediaPlayer::PlayingState:
        ui->pbPlay->setDown(true);
        break;
    }

}

void MediaPlayer::on_pbBack_clicked()
{
    if ( pPlayList->currentIndex() > 0 ) {
        pPlayList->previous();
    }
    qDebug() << "Player is at: " << pPlayList->currentIndex() << endl;
    setTextStatusbar();
}

void MediaPlayer::on_pbForward_clicked()
{
    if ( pPlayList->currentIndex() < (pPlayList->mediaCount()-1) ) {
        pPlayList->next();
    }
    qDebug() << "Player is at: " << pPlayList->currentIndex() << endl;
    setTextStatusbar();
}



void MediaPlayer::on_hzVolume_valueChanged(int _value)
{
    pPlayer->setVolume(_value);
}

void MediaPlayer::on_lTunes_doubleClicked(const QModelIndex &_index)
{
    int tuneNo = _index.row();
    pPlayList->setCurrentIndex(tuneNo);
    qDebug() << "Player is at: " << pPlayList->currentMedia().canonicalUrl().fileName() << endl;
    if( pPlayer->state() == QMediaPlayer::StoppedState)
        pPlayer->play();
    setTextStatusbar();
}




