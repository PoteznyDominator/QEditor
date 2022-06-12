#include <QApplication>
#include <QFile>
#include <QDebug>
#include "header/msg.h"
#include "header/MainWindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  auto* mainWindow = new MainWindow();

  QFile file(":/style/style.css");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    Msg::ERROR("Could not opened stylesheet", mainWindow);
  }

  const auto style = file.readAll();
  app.setStyleSheet(style);

  mainWindow->show();
  return app.exec();
}
