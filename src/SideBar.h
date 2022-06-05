//
// Created by PoteznyDominator on 04/06/2022.
//

#ifndef QEDITIOR_SIDEBAR_H
#define QEDITIOR_SIDEBAR_H

#include <QFileSystemModel>
#include <QTreeView>
#include <QMouseEvent>

class SideBar : public QTreeView {
  Q_OBJECT
public:
  explicit SideBar(const QString& rootPath, QWidget* parent = nullptr);

signals:
  void fileDoubleClick(const QString& filePath);

private:
  QFileSystemModel* model_;

  void printFileName(const QModelIndex& index);
  void mouseDoubleClickEvent(QMouseEvent *event) override;
};


#endif//QEDITIOR_SIDEBAR_H
