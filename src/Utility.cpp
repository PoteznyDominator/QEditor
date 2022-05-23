//
// Created by PoteznyDominator on 23.05.2022.
//

#include "Utility.h"
#include <QFileInfo>
#include <QTabBar>

QString Utils::getFileName(const QString &filePath) {
  const QFileInfo info(filePath);
  return info.fileName();
}
int Utils::getIndexOfOpenedFile(const QString &filePath, QTabWidget *tabWidget) {
  for (int i = 0; i < tabWidget->count(); i++) {
    return tabWidget->tabToolTip(i) == filePath ? i : 0;
  }
}
