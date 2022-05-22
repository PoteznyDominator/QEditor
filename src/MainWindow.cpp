//
// Created by PoteznyDominator on 22.05.2022.
//

#include "MainWindow.h"
#include "EditorWidget.h"
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setCentralWidget(new QWidget);
  initUI();
}

void MainWindow::initUI() {
  setMinimumSize(800, 600);

  auto *layout = new QBoxLayout(QBoxLayout::TopToBottom);
  auto *splitter = new QSplitter(this);

  treeWidget_ = new QTreeWidget;
  stackedWidget_ = new QStackedWidget;

  initWelcomeWidget();
  initTabWidget();

  stackedWidget_->addWidget(welcomeWidget_);
  stackedWidget_->addWidget(tabWidget_);
  stackedWidget_->setCurrentWidget(tabWidget_);

  splitter->addWidget(treeWidget_);
  splitter->addWidget(stackedWidget_);
  splitter->setOrientation(Qt::Horizontal);
  splitter->setCollapsible(0, true);
  treeWidget_->setMinimumWidth(100);

  layout->addWidget(splitter);

  centralWidget()->setLayout(layout);
}

void MainWindow::initWelcomeWidget() {
  welcomeWidget_ = new QWidget;

  auto *layout = new QBoxLayout(QBoxLayout::TopToBottom);
  auto *newFileBtn = new QPushButton("New file");
  auto *openFileBtn = new QPushButton("Open file");
  auto *welcomeLabel = new QLabel("Welcome to QEditor ;)");
  welcomeLabel->setAlignment(Qt::AlignCenter);

  layout->addStretch();
  layout->addWidget(welcomeLabel);
  layout->addWidget(newFileBtn);
  layout->addWidget(openFileBtn);
  layout->addStretch();

  welcomeWidget_->setLayout(layout);
}

void MainWindow::initTabWidget() {
  tabWidget_ = new QTabWidget;
  tabWidget_->addTab(new EditorWidget, "tab");
  tabWidget_->setMovable(true);
  tabWidget_->setTabsClosable(true);
}
