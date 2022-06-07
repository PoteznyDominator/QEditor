//
// Created by PoteznyDominator on 04/06/2022.
//

#include "../header/SideBar.h"
#include <QDebug>

SideBar::SideBar(const QString& rootPath, QWidget* parent) : QTreeView(parent) {
  setMinimumWidth(50);

  model_ = new QFileSystemModel(this);
  setModel(model_);
  setRootIndex(model_->setRootPath(rootPath));
  setHeaderHidden(true);
  // hiding unneeded columns
  // leaving only name of file/directory
  hideColumn(1);
  hideColumn(2);
  hideColumn(3);

  connect(this, &SideBar::doubleClicked, this, &SideBar::printFileName);
}

void SideBar::printFileName(const QModelIndex& index) {
  QFileInfo file(model_->filePath(index));
  if (file.isFile()) { emit fileDoubleClick(model_->filePath(index)); }
}

void SideBar::mouseDoubleClickEvent(QMouseEvent* event) {
  // for now, we only allow using left mouse double click to avoid opening file with right double click
  if (event->button() == Qt::LeftButton) {
    QTreeView::mouseDoubleClickEvent(event);
  }
}
