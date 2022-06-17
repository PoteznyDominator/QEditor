#include "header/MainWindow.h"
#include "header/msg.h"
#include <QApplication>
#include <QDebug>
#include <QFile>

void loadTheme(QString& styleSheet, MainWindow* mainWindow) {
  QFile file(":/theme/theme");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    Msg::ERROR("Could not opened stylesheet", mainWindow);
  }

  const QString theme = file.readAll();

  foreach (auto& line, theme.split(QString("\n"), Qt::SkipEmptyParts)) {
    QList<QString> themePairs = line.split("=", Qt::SkipEmptyParts);
    styleSheet.replace(themePairs.first().trimmed(),
                       themePairs.back().trimmed());
  }
}

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  auto* mainWindow = new MainWindow();

  QFile file(":/style/style.css");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    Msg::ERROR("Could not opened stylesheet", mainWindow);
  }

  QString style = file.readAll();
  // add theme preprocessing
  loadTheme(style, mainWindow);
  app.setStyleSheet(style);

  mainWindow->show();
  return app.exec();
}
