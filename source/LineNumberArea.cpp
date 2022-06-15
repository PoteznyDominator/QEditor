//
// Created by PoteznyDominator on 22.05.2022.
//

#include "../header/LineNumberArea.h"

LineNumberArea::LineNumberArea(EditorWidget* parent)
    : QWidget(parent), editorWidget(parent) {}

void LineNumberArea::paintEvent(QPaintEvent* event) {
  editorWidget->lineNumberAreaPaintEvent(event);
}

QSize LineNumberArea::sizeHint() const {
  return QSize(editorWidget->lineNumberAreaWidth(), 0);
}

void LineNumberArea::zoomIn(QFont font) { setFont(font); }

void LineNumberArea::zoomOut(QFont font) { setFont(font); }
