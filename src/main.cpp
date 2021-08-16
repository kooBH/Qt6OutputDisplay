#include <Qt>
#include <QApplication>
#include <QFontDatabase>
#include "display.h"

int main(int argc, char* argv[]) {
  QCoreApplication::addLibraryPath(".");
  QCoreApplication::addLibraryPath("../lib");
  QDir dir; 
  std::cout << dir.absolutePath().toStdString()<<std::endl;
  QApplication app(argc, argv);

  // Set Font
  int id = QFontDatabase::addApplicationFont("../font/AppleSDGothicNeoB.ttf");
  QString family = QFontDatabase::applicationFontFamilies(id).at(0);
  QFont SerifFont(family);
  app.setFont(SerifFont);

  Display demo;
  demo.show();

  return app.exec();
}