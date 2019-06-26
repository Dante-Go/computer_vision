#include "mainwindow.h"
#include "ui_mainwindow.h"

#define PLUGINS_SUBFOLDER               "/cvplugins/"
#define LANGUAGES_SUBFOLDER             "/languages/"
#define THEMES_SUBFOLDER                "/themes/"
#define FILE_ON_DISK_DYNAMIC_PROPERTY   "absolute_file_path"


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  loadSettings();

  populatePluginsMenu();
  populateLanguagesMenu();
  populateThemesMenu();

  ui->graphicsView->setScene(&scene);
  scene.addItem(&originalPixmap);
  scene.addItem(&processedPixmap);

  ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

MainWindow::~MainWindow()
{
  delete currentPlugin;
  delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  int result = QMessageBox::warning(this, tr("Exit"), tr("Are you sure want to exit?"), QMessageBox::Yes,
                                    QMessageBox::No);
  if(result == QMessageBox::Yes)
    {
      saveSettings();
      event->accept();
    }
  else {
      event->ignore();
    }
}

void MainWindow::changeEvent(QEvent *event)
{
  if(event->type() == QEvent::LanguageChange)
    {
      ui->retranslateUi(this);
    }
  else {
      QMainWindow::changeEvent(event);
    }
}

void MainWindow::onPluginActionTriggered(bool)
{
  if(!currentPlugin.isNull())
    {
      delete currentPlugin;
      delete currentPluginGui;
    }
  currentPluginFile = QObject::sender()->property(FILE_ON_DISK_DYNAMIC_PROPERTY).toString();
  currentPlugin = new QPluginLoader(currentPluginFile, this);
  currentPluginGui = new QWidget(this);
  ui->pluginLayout->addWidget(currentPluginGui);
  CvPluginInterface *currentPluginInstance = dynamic_cast<CvPluginInterface*>(currentPlugin->instance());
  if(currentPluginInstance)
    {
      currentPluginInstance->setupUi(currentPluginGui);
      connect(currentPlugin->instance(), SIGNAL(updateNeeded()), this, SLOT(onCurrentPluginUpdateNeeded()));
      connect(currentPlugin->instance(), SIGNAL(infoMessage(QString)), this, SLOT(onCurrentPluginInfoMessage(QString)));
      connect(currentPlugin->instance(), SIGNAL(errorMessage(QString)), this, SLOT(onCurrentPluginErrorMessage(QString)));
    }
}

void MainWindow::onLanguageActionTriggered(bool)
{

}

void MainWindow::onThemeActionTriggered(bool)
{

}

void MainWindow::onCurrentPluginUpdateNeeded()
{
  if(!originalMat.empty())
    {
      if(!currentPlugin.isNull())
        {
          CvPluginInterface *currentPluginInstance = dynamic_cast<CvPluginInterface*>(currentPlugin->instance());
          if(currentPluginInstance)
            {
              cv::TickMeter meter;
              meter.start();
              currentPluginInstance->processImage(originalMat, processedMat);
              meter.stop();
              qDebug() << "The process took " << meter.getTimeMilli() << "milliseconds";
            }
        }
      else {
          processedMat = originalMat.clone();
        }

      originalImage = QImage(originalMat.data, originalMat.cols, originalMat.rows, originalMat.step,
                             QImage::Format_RGB888);
      originalPixmap.setPixmap(QPixmap::fromImage(originalImage.rgbSwapped()));

      processedImage = QImage(processedMat.data, processedMat.cols, processedMat.rows, processedMat.step,
                              QImage::Format_RGB888);
      processedPixmap.setPixmap(QPixmap::fromImage(processedImage.rgbSwapped()));
    }
}

void MainWindow::onCurrentPluginErrorMessage(QString msg)
{
  qDebug() << "Plugin Error Message : " << msg;
}

void MainWindow::onCurrentPluginInfoMessage(QString msg)
{
  qDebug() << "Plugin Info Message : " << msg;
}

void MainWindow::on_action_Open_Image_triggered()
{
  QString fileName = QFileDialog::getOpenFileName(this,
                                                  tr("Open Input Image"),
                                                  QDir::currentPath(),
                                                  tr("(*.jpg *.png *.bmp)"));

  using namespace cv;
//  originalMat = imread(fileName.toStdString());
  originalMat = imread(fileName.toLocal8Bit().toStdString()); // 支持中文路径
  if(!originalMat.empty())
    {
      onCurrentPluginUpdateNeeded();
    }
  else if (!fileName.trimmed().isEmpty()) {
      QMessageBox::critical(this,
                            tr("Error"),
                            tr("Make sure the image file exists"));
    }

}

void MainWindow::on_action_Save_Image_triggered()
{
  if(!ui->viewOriginalCheck->isChecked() && !processedMat.empty())
    {
      QString fileName = QFileDialog::getSaveFileName(this,
                                                      tr("Save Iamge"),
                                                      QDir::currentPath(),
                                                      "*.jpg;;*.png;;*.bmp");
      if(!fileName.isEmpty())
        {
//          cv::imwrite(fileName.toStdString(), processedMat);
          cv::imwrite(fileName.toLocal8Bit().toStdString(), processedMat);
        }
    }
  else if (ui->viewOriginalCheck->isChecked() && !originalMat.empty()) {
      QString fileName = QFileDialog::getSaveFileName(this,
                                                      tr("Save Image"),
                                                      QDir::currentPath(),
                                                      "*.jpg;;*.png;;*.bmp");
      if(!fileName.isEmpty())
        {
//          cv::imwrite(fileName.toStdString(), originalMat);
          cv::imwrite(fileName.toLocal8Bit().toStdString(), originalMat);
        }
    }
  else {
      QMessageBox::warning(this,
                           tr("Warning"),
                           tr("There is nothing to be saved!"));
    }
}

void MainWindow::on_action_Exit_triggered()
{
  close();
}

void MainWindow::on_viewOriginalCheck_toggled(bool checked)
{
  originalPixmap.setVisible(checked);
  processedPixmap.setVisible(!checked);
}

void MainWindow::on_actionAbout_triggered()
{
  qApp->aboutQt();
}

void MainWindow::loadSettings()
{
  QSettings settings("Packt", "Computer_Vision", this);
  currentThemeFile = settings.value("currentThemeFile", "").toString();
  currentLanguageFile = settings.value("currentLanguageFile", "").toString();
  currentPluginFile = settings.value("currentPluginFile", "").toString();
}

void MainWindow::saveSettings()
{
  QSettings settings("Packt", "Computer_Vision", this);
  settings.setValue("currentThemeFile", currentThemeFile);
  settings.setValue("currentLanguageFile", currentLanguageFile);
  settings.setValue("currentPluginFile", currentPluginFile);

}

void MainWindow::populateThemesMenu()
{

}

void MainWindow::populateLanguagesMenu()
{

}

void MainWindow::populatePluginsMenu()
{
  // load all plugins and populate the menus
  QDir pluginsDir(qApp->applicationDirPath() + PLUGINS_SUBFOLDER);
  QFileInfoList pluginFiles = pluginsDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files,
                                                       QDir::Name);
  foreach(QFileInfo pluginFile, pluginFiles)
    {
      if(QLibrary::isLibrary(pluginFile.absoluteFilePath()))
        {
          QPluginLoader pluginLoader(pluginFile.absoluteFilePath(), this);
          if(CvPluginInterface *plugin = dynamic_cast<CvPluginInterface*>(pluginLoader.instance()))
            {
              QAction *pluginAction = ui->menu_Plugins->addAction(plugin->title());
              pluginAction->setProperty(FILE_ON_DISK_DYNAMIC_PROPERTY, pluginFile.absoluteFilePath());
              connect(pluginAction, SIGNAL(triggered(bool)), this, SLOT(onPluginActionTriggered(bool)));
              if(currentPluginFile == pluginFile.absoluteFilePath())
                {
                  pluginAction->trigger();
                }
            }
          else {
              QMessageBox::warning(this,
                                   tr("Waring"),
                                   QString(tr("Make sure %1 is a correct plugin for this application<br>"
                                              "and it's not in use by some other application!"))
                                   .arg(pluginFile.fileName()));
            }
        }
      else {
          QMessageBox::warning(this,
                               tr("Warning"),
                               QString(tr("Make sure only plugins exist in %1 folder.<br>"
                                          "%2 is not a plugin."))
                               .arg(PLUGINS_SUBFOLDER)
                               .arg(pluginFile.fileName()));
        }
    }

  if(ui->menu_Plugins->actions().count() <= 0)
    {
      QMessageBox::critical(this,
                            tr("No Plugins"),
                            QString(tr("This application cannot work without plugins!"
                                       "<br>Make sure that %1 folder exists "
                                       "in the same folder as the application<br> and that"
                                       " there are some filter plugins inside it"))
                            .arg(PLUGINS_SUBFOLDER));
      this->setEnabled(false);
    }
}
