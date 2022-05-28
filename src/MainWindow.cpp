//
// Created by PoteznyDominator on 22.05.2022.
//

#include "MainWindow.h"
#include "EditorWidget.h"
#include "Utility.h"
#include "msg.h"
#include <QAction>
#include <QBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QSplitter>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  setCentralWidget(new QWidget);
  initUI();
  initMenuBar();
  initStyleSheet();
}

void MainWindow::initUI() {
  setMinimumSize(800, 600);

  auto* layout = new QBoxLayout(QBoxLayout::TopToBottom);
  auto* splitter = new QSplitter(this);

  treeWidget_ = new QTreeWidget;
  stackedWidget_ = new QStackedWidget;

  initWelcomeWidget();
  initTabWidget();

  stackedWidget_->addWidget(welcomeWidget_);
  stackedWidget_->addWidget(tabWidget_);
  stackedWidget_->setCurrentWidget(welcomeWidget_);

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

  auto* layout = new QBoxLayout(QBoxLayout::TopToBottom);
  auto* newFileBtn = new QPushButton("New file");
  auto* openFileBtn = new QPushButton("Open file");
  auto* welcomeLabel = new QLabel("Welcome to QEditor ;)");
  welcomeLabel->setAlignment(Qt::AlignCenter);

  connect(openFileBtn, &QPushButton::clicked, this, &MainWindow::openFile);
  connect(newFileBtn, &QPushButton::clicked, this, &MainWindow::newFile);

  layout->addStretch();
  layout->addWidget(welcomeLabel);
  layout->addWidget(newFileBtn);
  layout->addWidget(openFileBtn);
  layout->addStretch();

  welcomeWidget_->setLayout(layout);
}

void MainWindow::initTabWidget() {
  tabWidget_ = new QTabWidget;
  tabWidget_->setMovable(true);
  tabWidget_->setTabsClosable(true);
  connect(tabWidget_, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
}
QAction* MainWindow::getAction(const QString& text, QKeySequence shortCut,
                               void (MainWindow::*slot)()) {
  auto* action = new QAction(text, this);
  // skipping shortcut if doesn't exist
  if (!shortCut.isEmpty()) action->setShortcut(shortCut);
  // connecting function to action
  if (slot) connect(action, &QAction::triggered, this, slot);
  return action;
}

void MainWindow::initMenuBar() {
  auto* menuBar = new QMenuBar(this);

  auto* fileSection = new QMenu("&File");
  auto* editSection = new QMenu("&Edit");
  auto* optionsSection = new QMenu("&Options");
  auto* helpSection = new QMenu("&Help");

  fileSection->addAction(getAction("New", QKeySequence::New, &MainWindow::newFile));
  fileSection->addAction(getAction("Open", QKeySequence::Open, &MainWindow::openFile));
  fileSection->addAction(getAction("Save", QKeySequence::Save, &MainWindow::saveFile));
  fileSection->addAction(
    getAction("Save as", QKeySequence("Ctrl+Shift+s"), &MainWindow::saveFileAs));

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

void MainWindow::initStyleSheet() {
  QFile file("../style/style.css");
  file.open(QFile::ReadOnly);
  QString styleSheet = QLatin1String(file.readAll());

  setStyleSheet(styleSheet);
}

void MainWindow::openFile() {
  auto filePath = QFileDialog::getOpenFileName(this, "Open file");
  if (!filePath.isEmpty()) {
    QFile file(filePath);
    // check if file can be opened
    if (!file.open(QIODevice::Text | QIODevice::ReadOnly)) {
      Msg::ERROR(QString("Could not open file: \"%1\"").arg(filePath), this);
    }

    QTextStream in(&file);
    auto content = in.readAll();
    file.close();

    addFileToTabWidget(filePath, content);
  }
}

void MainWindow::newFile() { addFileToTabWidget("Untitled"); }
void MainWindow::addFileToTabWidget(const QString& filePath, const QString& fileContent) {
  if (stackedWidget_->currentWidget() == welcomeWidget_)
    stackedWidget_->setCurrentWidget(tabWidget_);

  const auto fileName = Utils::getFileName(filePath);

  // if file is already opened set it as current widget
  int index = Utils::getIndexOfOpenedFile(filePath, tabWidget_);
  if (index) {
    tabWidget_->setCurrentIndex(index);
    return;
  }

  auto* editor = new EditorWidget(tabWidget_);
  editor->setPlainText(fileContent);
  tabWidget_->addTab(editor, fileName);
  tabWidget_->setTabToolTip(tabWidget_->count() - 1, filePath);
  tabWidget_->setCurrentIndex(tabWidget_->count() - 1);
}

void MainWindow::saveFile() {
  const auto currentFile = dynamic_cast<EditorWidget*>(tabWidget_->currentWidget());

  if (!currentFile || !currentFile->isChanged()) {
    // maybe print some error?
    return;
  }

  executeSavingFile(tabWidget_->tabToolTip(tabWidget_->currentIndex()));
}

void MainWindow::saveFileAs() {
  const auto filePath = QFileDialog::getSaveFileName(this, "Save file as");

  if (!filePath.isEmpty()) executeSavingFile(filePath);
}

void MainWindow::executeSavingFile(const QString& filePath) {
  QFile file(filePath);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    Msg::ERROR(QString("Could not save file: \"%1\"").arg(file.fileName()), this);
  }

  QTextStream out(&file);
  const auto currentFile = dynamic_cast<EditorWidget*>(tabWidget_->currentWidget());
  out << currentFile->toPlainText();
}
void MainWindow::closeTab(int index) {
  tabWidget_->removeTab(index);

  // TODO: add warning when user want to close changed file

  // changing to welcomeWidget when no file is opened
  if (tabWidget_->count() == 0) stackedWidget_->setCurrentWidget(welcomeWidget_);
}
