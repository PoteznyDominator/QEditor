//
// Created by PoteznyDominator on 22.05.2022.
//

#include "EditorWidget.h"
#include "LineNumberArea.h"
#include <QPainter>
#include <QPlainTextEdit>
#include <QTextBlock>


EditorWidget::EditorWidget(QWidget *parent) : QPlainTextEdit(parent) {
  setWordWrapMode(QTextOption::NoWrap);

  lineNumberArea = new LineNumberArea(this);
  //  connecting usefully signal for the linenumberarea
  connect(this, &EditorWidget::blockCountChanged, this, &EditorWidget::updateLineNumberAreaWidth);
  connect(this, &EditorWidget::updateRequest, this, &EditorWidget::updateLineNumberArea);
  connect(this, &EditorWidget::cursorPositionChanged, this, &EditorWidget::highlightCurrentLine);

  updateLineNumberAreaWidth(0);
  highlightCurrentLine();
}

void EditorWidget::lineNumberAreaPaintEvent(QPaintEvent *event) {
  QPainter painter(lineNumberArea);
  painter.fillRect(event->rect(), Qt::lightGray);
  QTextBlock block = firstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
  int bottom = top + qRound(blockBoundingRect(block).height());
  while (block.isValid() && top <= event->rect().bottom()) {
    if (block.isVisible() && bottom >= event->rect().top()) {
      QString number = QString::number(blockNumber + 1);
      painter.setPen(Qt::black);
      painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight,
                       number);
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

  int space = 5 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

  return space;
}

void EditorWidget::resizeEvent(QResizeEvent *e) {
  QPlainTextEdit::resizeEvent(e);

  QRect cr = contentsRect();
  lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void EditorWidget::updateLineNumberAreaWidth(int newBlockCount) {
  setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void EditorWidget::highlightCurrentLine() {
  QList<QTextEdit::ExtraSelection> extraSelections;

  if (!isReadOnly()) {
    QTextEdit::ExtraSelection selection;

    QColor lineColor = QColor(Qt::yellow).lighter(160);

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);
  }

  setExtraSelections(extraSelections);
}

void EditorWidget::updateLineNumberArea(const QRect &rect, int dy) {
  if (dy) lineNumberArea->scroll(0, dy);
  else
    lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

  if (rect.contains(viewport()->rect())) updateLineNumberAreaWidth(0);
}
