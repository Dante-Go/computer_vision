#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDir>
#include <QFileInfoList>
#include <QLibrary>
#include <QPluginLoader>
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QGraphicsScene>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QTranslator>
#include <QThread>
#include <QThreadPool>
#include <QRunnable>
#include <QRunnable>
#include <QMutex>
#include <QWaitCondition>

#include "cvplugininterface.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

protected:
  void closeEvent(QCloseEvent *event);
  void changeEvent(QEvent *event);

private slots:
  void onPluginActionTriggered(bool);
  void onLanguageActionTriggered(bool);
  void onThemeActionTriggered(bool);
  void onCurrentPluginUpdateNeeded();
  void onCurrentPluginErrorMessage(QString msg);
  void onCurrentPluginInfoMessage(QString msg);

  void on_action_Open_Image_triggered();

  void on_action_Save_Image_triggered();

  void on_action_Exit_triggered();

  void on_viewOriginalCheck_toggled(bool checked);

  void on_actionAbout_triggered();

private:
  Ui::MainWindow *ui;

  void loadSettings();
  void saveSettings();

  QString currentThemeFile;
  QString currentLanguageFile;
  QString currentPluginFile;

  void populateThemesMenu();
  void populateLanguagesMenu();
  void populatePluginsMenu();

  QPointer<QPluginLoader> currentPlugin;
  QPointer<QWidget> currentPluginGui;
  QGraphicsScene scene;
  QTranslator translator;
  QGraphicsPixmapItem originalPixmap;
  QGraphicsPixmapItem processedPixmap;
  cv::Mat originalMat;
  cv::Mat processedMat;
  QImage originalImage;
  QImage processedImage;
};

#endif // MAINWINDOW_H
