#include "template_plugin.h"

#include "ui_plugin.h"

Template_plugin::Template_plugin()
{
}

Template_plugin::~Template_plugin()
{

}

QString Template_plugin::title()
{
  return this->metaObject()->className();
}

QString Template_plugin::version()
{
  return "1.0.0";
}

QString Template_plugin::description()
{
  return "A <b>Template</b> plugin";
}

QString Template_plugin::help()
{
  return "This is a <b>Template</b> plugin. Clone and use it to create new plugins.";
}

void Template_plugin::setupUi(QWidget *parent)
{
  ui = new Ui::PluginGui;
  ui->setupUi(parent);
}

void Template_plugin::processImage(const cv::Mat &inputImage, cv::Mat &outputImage)
{
  inputImage.copyTo(outputImage);
}



