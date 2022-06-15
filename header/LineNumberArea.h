//
// Created by PoteznyDominator on 22.05.2022.
//

#ifndef QEDITIOR_LINENUMBERAREA_H
#define QEDITIOR_LINENUMBERAREA_H

#include "EditorWidget.h"
#include <QWidget>

class LineNumberArea : public QWidget {
  Q_OBJECT
public:
  explicit LineNumberArea(EditorWidget* parent = nullptr);

  QSize sizeHint() const override;
  void zoomIn(QFont font);
  void zoomOut(QFont font);

protected:
  void paintEvent(QPaintEvent* event) override;

private:
  EditorWidget* editorWidget;
};

#endif//QEDITIOR_LINENUMBERAREA_H
