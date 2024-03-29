#include "copymakeborder_plugin.h"

#include "ui_plugin.h"

CopyMakeBorder_Plugin::CopyMakeBorder_Plugin()
{
}

CopyMakeBorder_Plugin::~CopyMakeBorder_Plugin()
{

}

QString CopyMakeBorder_Plugin::title()
{
  return this->metaObject()->className();
}

QString CopyMakeBorder_Plugin::version()
{
  return "1.0.0";
}

QString CopyMakeBorder_Plugin::description()
{
  return "A <b>Template</b> plugin";
}

QString CopyMakeBorder_Plugin::help()
{
  return "This is a <b>Template</b> plugin. Clone and use it to create new plugins.";
}

void CopyMakeBorder_Plugin::setupUi(QWidget *parent)
{
  ui = new Ui::PluginGui;
  ui->setupUi(parent);
  QStringList items;
  items.append(tr("BORDER_CONSTANT"));
  items.append(tr("BORDER_REPLICATE"));
  items.append(tr("BORDER_REFLECT"));
  items.append(tr("BORDER_WRAP"));
  items.append(tr("BORDER_REFLECT_101"));
  ui->borderTypeComboBox->addItems(items);
  connect(ui->borderTypeComboBox,
          SIGNAL(currentIndexChanged(int)),
          this,
          SLOT(on_borderTypeComboBox_currentIndexChanged(int)));
}

void CopyMakeBorder_Plugin::processImage(const cv::Mat &inputImage, cv::Mat &outputImage)
{
  int top, bottom, left, right;
  top = bottom = inputImage.rows/2;
  left = right = inputImage.cols/2;
  cv::copyMakeBorder(inputImage,
                     outputImage,
                     top,
                     bottom,
                     left,
                     right,
                     ui->borderTypeComboBox->currentIndex());
}




void CopyMakeBorder_Plugin::on_borderTypeComboBox_currentIndexChanged(int index)
{
    emit updateNeeded();
}
