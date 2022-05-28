//
// Created by PoteznyDominator on 28/05/2022.
//

#ifndef QEDITIOR_MSG_H
#define QEDITIOR_MSG_H

#include <QMessageBox>

class Msg : public QMessageBox {
  Q_OBJECT
public:
  static void ERROR(const QString& message, QWidget* parent = nullptr);
  static void WARN(const QString& message, QWidget* parent = nullptr);
  static void INFO(const QString& message, QWidget* parent = nullptr);

private:
  enum Type {
    Error,
    Info,
    Warn,
  };

  explicit Msg(const QString& message, Msg::Type type, QWidget* parent = nullptr);
  void resizeEvent(QResizeEvent* event) override;
  void showEvent(QShowEvent* event) override;
};


#endif//QEDITIOR_MSG_H
