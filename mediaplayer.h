#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QMainWindow>
#include <QPointer>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QUrl>
#include <QList>
#include <QString>
#include <QStringList>
#include <QStringListModel>

namespace Ui {
class MediaPlayer;
}

class MediaPlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit MediaPlayer(QWidget *parent = nullptr);
    ~MediaPlayer();

private slots:
    void on_actionAdd_files_triggered();
    void on_actionClear_list_triggered();
    void on_actionSave_list_triggered();
    void on_actionload_list_triggered();
    void on_pbPlay_clicked();
    void on_pbStop_clicked();
    void on_pbBack_clicked();
    void on_pbForward_clicked();
    void on_state_changed(QMediaPlayer::State _state);
    void on_hzVolume_valueChanged(int _value);
    void on_lTunes_doubleClicked(const QModelIndex &index);

private:
    Ui::MediaPlayer *ui;

    // Functions
    void setFilesToPlayer(QList<QUrl> _listFiles, QString &_error);
    void setFilesToModel(QVariant _fileName, QString &_error);
    void setTextStatusbar();

    // Media classes
    QPointer<QMediaPlayer> pPlayer;
    QPointer<QMediaPlaylist> pPlayList;

    // GUI classes
    QPointer<QAction> pAddFiles;
    QPointer<QAction> pSaveList;
    QPointer<QAction> pLoadList;
    QPointer<QAction> pExit;

    // Main classes
    QStringList listTunes;
    QPointer<QStringListModel> pListModel;

};

#endif // MEDIAPLAYER_H
