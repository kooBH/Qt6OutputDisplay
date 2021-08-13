#include <Qt>
#include <QApplication>
#include "display.h"

int main(int argc, char* argv[]) {
  QCoreApplication::addLibraryPath(".");
  QCoreApplication::addLibraryPath("../lib");

  QApplication app(argc, argv);

  Display demo;
  demo.show();

  return app.exec();
}