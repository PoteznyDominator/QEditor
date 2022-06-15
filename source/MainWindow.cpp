//
// Created by PoteznyDominator on 22.05.2022.
//

#include "../header/MainWindow.h"
#include "../header/EditorWidget.h"
#include "../header/Utility.h"
#include "../header/msg.h"
#include <QAction>
#include <QBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QToolButton>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), stackedWidget_(nullptr), tabWidget_(nullptr),
      sideBar_(nullptr), welcomeWidget_(nullptr), splitter_(nullptr) {
  setCentralWidget(new QWidget(this));
  layout()->setContentsMargins(0, 0, 0, 0);
  initUI();
  initMenuBar();
}

void MainWindow::initUI() {
  setMinimumSize(800, 600);

  auto* layout = new QBoxLayout(QBoxLayout::TopToBottom);
  splitter_ = new QSplitter(this);

  stackedWidget_ = new QStackedWidget;

  initWelcomeWidget();
  initTabWidget();

  stackedWidget_->addWidget(welcomeWidget_);
  stackedWidget_->addWidget(tabWidget_);
  stackedWidget_->setCurrentWidget(welcomeWidget_);

  splitter_->addWidget(stackedWidget_);
  splitter_->setOrientation(Qt::Horizontal);

  layout->addWidget(splitter_);
  layout->setContentsMargins(0, 0, 0, 0);
  centralWidget()->setLayout(layout);
}

void MainWindow::initWelcomeWidget() {
  welcomeWidget_ = new QWidget;
  welcomeWidget_->setObjectName("WelcomeWidget");

  auto* layout = new QVBoxLayout();
  layout->setAlignment(Qt::AlignHCenter);
  auto* newFileBtn = new QPushButton("New file", welcomeWidget_);
  auto* openFileBtn = new QPushButton("Open file", welcomeWidget_);
  auto* welcomeLabel = new QLabel("Welcome to QEditor ;)");
  welcomeLabel->setObjectName("WelcomeLabel");

  newFileBtn->setCursor(Qt::PointingHandCursor);
  openFileBtn->setCursor(Qt::PointingHandCursor);

  connect(openFileBtn, &QPushButton::clicked, this,
          qOverload<>(&MainWindow::openFile));
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
  connect(tabWidget_, &QTabWidget::tabCloseRequested, this,
          &MainWindow::closeTab);
}

void MainWindow::initSideBar() {
  sideBar_ = new SideBar(QDir::currentPath(), splitter_);
  connect(sideBar_, &SideBar::fileDoubleClick, this,
          qOverload<const QString&>(&MainWindow::openFile));

  // setting splitter with new widget
  splitter_->insertWidget(0, sideBar_);
  splitter_->setCollapsible(0, false);
  // by default this should always open sidebar with 150 width
  splitter_->setSizes(QList<int>{150, tabWidget_->width() - 150});
}

void MainWindow::initMenuBar() {
  auto* menuBar = new QMenuBar(this);

  auto* fileSection = new QMenu("&File");
  auto* editSection = new QMenu("&Edit");
  auto* optionsSection = new QMenu("&Options");
  auto* viewSection = new QMenu("&View");
  auto* helpSection = new QMenu("&Help");

  fileSection->addAction(
    createAction("New", &MainWindow::newFile, QKeySequence::New));
  fileSection->addAction(
    createAction("Open", &MainWindow::openFile, QKeySequence::Open));
  fileSection->addAction(
    createAction("Save", &MainWindow::saveFile, QKeySequence::Save));
  fileSection->addAction(createAction("Save as", &MainWindow::saveFileAs,
                                      QKeySequence("Ctrl+Shift+s")));

  editSection->addAction(createAction("Undo", QKeySequence::Undo));
  editSection->addAction(createAction("Redo", QKeySequence::Redo));
  editSection->addAction(createAction("Cut", QKeySequence::Cut));
  editSection->addAction(createAction("Copy", QKeySequence::Copy));
  editSection->addAction(createAction("Paste", QKeySequence::Paste));
  editSection->addAction(createAction("Find", QKeySequence::Find));
  editSection->addAction(
    createAction("Zoom in", &MainWindow::zoomIn, QKeySequence::ZoomIn));

  editSection->addAction(
    createAction("Zoom out", &MainWindow::zoomOut, QKeySequence::ZoomOut));

  viewSection->addAction(
    createAction("Show side bar", &MainWindow::showSideBar, true));

  menuBar->addMenu(fileSection);
  menuBar->addMenu(editSection);
  menuBar->addMenu(viewSection);
  menuBar->addMenu(optionsSection);
  menuBar->addMenu(helpSection);

  setMenuBar(menuBar);
}

void MainWindow::openFile() {
  auto filePath = QFileDialog::getOpenFileName(this, "Open file");
  if (!filePath.isEmpty()) openFile(filePath);
}
// overload function that opens file
void MainWindow::openFile(const QString& filePath) {
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

void MainWindow::newFile() { addFileToTabWidget("Untitled"); }
void MainWindow::addFileToTabWidget(const QString& filePath,
                                    const QString& fileContent) {
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
  connect(editor, &EditorWidget::zoomInSignal, this, &MainWindow::zoomIn);
  connect(editor, &EditorWidget::zoomOutSignal, this, &MainWindow::zoomOut);

  tabWidget_->addTab(editor, fileName);
  tabWidget_->setTabToolTip(tabWidget_->count() - 1, filePath);
  tabWidget_->setCurrentIndex(tabWidget_->count() - 1);
}
void MainWindow::saveFile() {
  const auto currentFile =
    dynamic_cast<EditorWidget*>(tabWidget_->currentWidget());

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

void MainWindow::zoomIn() {
  QList<EditorWidget*> filesList = tabWidget_->findChildren<EditorWidget*>();

  foreach (auto& tab, filesList) { tab->zoomIn(1); }
}

void MainWindow::zoomOut() {
  QList<EditorWidget*> filesList = tabWidget_->findChildren<EditorWidget*>();

  foreach (auto& tab, filesList) { tab->zoomOut(1); }
}

void MainWindow::executeSavingFile(const QString& filePath) {
  QFile file(filePath);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    Msg::ERROR(QString("Could not save file: \"%1\"").arg(file.fileName()),
               this);
  }

  QTextStream out(&file);
  const auto currentFile =
    dynamic_cast<EditorWidget*>(tabWidget_->currentWidget());
  out << currentFile->toPlainText();
}

void MainWindow::closeTab(int index) {
  tabWidget_->removeTab(index);

  // TODO: add warning when user want to close changed file

  // changing to welcomeWidget when no file is opened
  if (tabWidget_->count() == 0)
    stackedWidget_->setCurrentWidget(welcomeWidget_);
}

QAction* MainWindow::createAction(const QString& text,
                                  const QKeySequence& shortCut) {
  if (text.isEmpty()) return nullptr;

  auto* action = new QAction(text, this);
  if (!shortCut.isEmpty()) action->setShortcut(shortCut);

  return action;
}

QAction* MainWindow::createAction(const QString& text,
                                  void (MainWindow::*slot)(),
                                  const QKeySequence& shortCut) {
  auto* action = createAction(text, shortCut);
  if (!action) return nullptr;

  if (slot) connect(action, &QAction::triggered, this, slot);
  return action;
}

QAction* MainWindow::createAction(const QString& text,
                                  void (MainWindow::*slot)(bool), bool checked,
                                  const QKeySequence& shortCut) {
  auto* action = createAction(text, shortCut);
  if (!action) return nullptr;

  if (checked) {
    action->setCheckable(true);
    connect(action, &QAction::toggled, this, slot);
  }

  return action;
}
void MainWindow::showSideBar(bool checked) {

  if (checked) {
    if (!sideBar_) {
      initSideBar();
      return;
    }

    splitter_->widget(0)->show();
    return;
  }

  splitter_->widget(0)->hide();
}
