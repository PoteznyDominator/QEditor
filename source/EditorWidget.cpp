//
// Created by PoteznyDominator on 22.05.2022.
//

#include "../header/EditorWidget.h"
#include "../header/LineNumberArea.h"
#include <QDebug>
#include <QPainter>
#include <QPlainTextEdit>
#include <QTextBlock>


EditorWidget::EditorWidget(QWidget* parent)
    : QPlainTextEdit(parent), isChanged_(false) {
  setWordWrapMode(QTextOption::NoWrap);

  lineNumberArea_ = new LineNumberArea(this);
  //  connecting usefully signal for the linenumberarea
  connect(this, &EditorWidget::blockCountChanged, this,
          &EditorWidget::updateLineNumberAreaWidth);
  connect(this, &EditorWidget::updateRequest, this,
          &EditorWidget::updateLineNumberArea);
  connect(this, &EditorWidget::cursorPositionChanged, this,
          &EditorWidget::highlightCurrentLine);
  connect(this, &EditorWidget::textChanged, this,
          [this] { isChanged_ = true; });

  updateLineNumberAreaWidth(0);
  highlightCurrentLine();
}

void EditorWidget::lineNumberAreaPaintEvent(QPaintEvent* event) {
  QPainter painter(lineNumberArea_);
  painter.fillRect(event->rect(), "#282828");
  QTextBlock block = firstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top =
    qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
  int bottom = top + qRound(blockBoundingRect(block).height());
  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      QString number = QString::number(blockNumber + 1);
      painter.setPen("#D8D8D8");
      painter.drawText(0, top, lineNumberArea_->width(), fontMetrics().height(),
                       Qt::AlignRight, number);
    }

    block = block.next();
    top = bottom;
    bottom = top + qRound(blockBoundingRect(block).height());
    ++blockNumber;
  }
}

int EditorWidget::lineNumberAreaWidth() {
  int digits = 1;
  int max = qMax(1, blockCount());
  while (max >= 10) {
    max /= 10;
    ++digits;
  }

  int baseSpace = 10;
  int space =
    baseSpace + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

  return space;
}

void EditorWidget::resizeEvent(QResizeEvent* e) {
  QPlainTextEdit::resizeEvent(e);

  QRect cr = contentsRect();
  lineNumberArea_->setGeometry(
    QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void EditorWidget::wheelEvent(QWheelEvent* event) {
  QPlainTextEdit::wheelEvent(event);

  // adding modifers for zooming in file
  if (event->modifiers() == Qt::ControlModifier) {
    if (event->delta() > 0) {
      //      zoomIn(2);
      emit zoomInSignal();
      return;
    }
    emit zoomOutSignal();
    //    zoomOut(2);
  }
}

void EditorWidget::updateLineNumberAreaWidth(int newBlockCount) {
  setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void EditorWidget::highlightCurrentLine() {
  QList<QTextEdit::ExtraSelection> extraSelections;

  if (!isReadOnly()) {
    QTextEdit::ExtraSelection selection;


    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);
  }

  setExtraSelections(extraSelections);
}

void EditorWidget::updateLineNumberArea(const QRect& rect, int dy) {
  if (dy) lineNumberArea_->scroll(0, dy);
  else
    lineNumberArea_->update(0, rect.y(), lineNumberArea_->width(),
                            rect.height());

  if (rect.contains(viewport()->rect())) updateLineNumberAreaWidth(0);
}
bool EditorWidget::isChanged() const { return isChanged_; }

void EditorWidget::zoomIn(int range) {
  QPlainTextEdit::zoomIn(range);
  dynamic_cast<LineNumberArea*>(lineNumberArea_)->zoomIn(font());
}

void EditorWidget::zoomOut(int range) {
  // avoiding too small font
  if (font().pointSize() <= 3) return;

  QPlainTextEdit::zoomOut(range);
  dynamic_cast<LineNumberArea*>(lineNumberArea_)->zoomOut(font());
}
