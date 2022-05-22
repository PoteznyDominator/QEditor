//
// Created by PoteznyDominator on 22.05.2022.
//

#include "MainWindow.h"
#include "EditorWidget.h"
#include <QAction>
#include <QBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  setCentralWidget(new QWidget);
  initUI();
  initMenuBar();
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
QAction *MainWindow::getAction(const QString &text, QKeySequence shortCut) {
  auto *action = new QAction(text, this);
  if (!shortCut.isEmpty()) { action->setShortcut(shortCut); }
  return action;
}
void MainWindow::initMenuBar() {
  auto *menuBar = new QMenuBar(this);

  auto* fileSection = new QMenu("&File");
  auto* editSection = new QMenu("&Edit");
  auto* optionsSection = new QMenu("&Options");
  auto* helpSection = new QMenu("&Help");

  fileSection->addAction(getAction("New", QKeySequence::New));
  fileSection->addAction(getAction("Open", QKeySequence::Open));
  fileSection->addAction(getAction("Save", QKeySequence::Save));
  fileSection->addAction(getAction("Save as", QKeySequence("Ctrl+Shift+s")));

  editSection->addAction(getAction("Undo", QKeySequence::Undo));
  editSection->addAction(getAction("Redo", QKeySequence::Redo));
  editSection->addAction(getAction("Cut", QKeySequence::Cut));
  editSection->addAction(getAction("Copy", QKeySequence::Copy));
  editSection->addAction(getAction("Paste", QKeySequence::Paste));
  editSection->addAction(getAction("Find", QKeySequence::Find));

  menuBar->addMenu(fileSection);
  menuBar->addMenu(editSection);
  menuBar->addMenu(optionsSection);
  menuBar->addMenu(helpSection);

  setMenuBar(menuBar);
}
