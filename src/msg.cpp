//
// Created by PoteznyDominator on 28/05/2022.
//

#include "msg.h"
Msg::Msg(const QString& message, Msg::Type type, QWidget* parent) {
  setText(message);
  switch (type) {
    case Error: {
      setIcon(QMessageBox::Critical);
      QMessageBox::setWindowTitle("QEditor Error");
      break;
    }
    case Info: {
      setIcon(QMessageBox::Information);
      QMessageBox::setWindowTitle("QEditor Information");
      break;
    }
    case Warn: {
      setIcon(QMessageBox::Warning);
      QMessageBox::setWindowTitle("QEditor Warning");
      break;
    }
    default: {
      break;
    }
  }
  exec();
}

void Msg::ERROR(const QString& message, QWidget* parent) {
  Msg(message, Msg::Type::Error, parent);
}
void Msg::WARN(const QString& message, QWidget* parent) {
  Msg(message, Msg::Type::Warn, parent);
}
void Msg::INFO(const QString& message, QWidget* parent) {
  Msg(message, Msg::Type::Info, parent);
}

void Msg::resizeEvent(QResizeEvent* event) {
  QMessageBox::resizeEvent(event);
  setMinimumWidth(200);
}
void Msg::showEvent(QShowEvent* event) {
  QMessageBox::showEvent(event);
  setMinimumWidth(200);
}
