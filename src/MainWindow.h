//
// Created by PoteznyDominator on 22.05.2022.
//

#ifndef QEDITIOR_MAINWINDOW_H
#define QEDITIOR_MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QTreeWidget>

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = nullptr);

private:
  QStackedWidget *stackedWidget_;
  QTabWidget *tabWidget_;
  QTreeWidget *treeWidget_;
  QWidget *welcomeWidget_;

  void initUI();
  void initWelcomeWidget();
  void initTabWidget();
};


#endif//QEDITIOR_MAINWINDOW_H
