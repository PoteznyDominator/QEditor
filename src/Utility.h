//
// Created by PoteznyDominator on 23.05.2022.
//

#ifndef QEDITIOR_UTILITY_H
#define QEDITIOR_UTILITY_H

#include <QString>
#include <QTabWidget>

namespace Utils {
  QString getFileName(const QString& filePath);

  int getIndexOfOpenedFile(const QString& filePath, QTabWidget* tabWidget);
}// namespace Utils


#endif//QEDITIOR_UTILITY_H
