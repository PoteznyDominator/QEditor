//
// Created by PoteznyDominator on 22.05.2022.
//

#ifndef QEDITIOR_EDITORWIDGET_H
#define QEDITIOR_EDITORWIDGET_H

#include <QPlainTextEdit>

class EditorWidget : public QPlainTextEdit {
  Q_OBJECT
public:
  EditorWidget(QWidget *parent = nullptr);

  void lineNumberAreaPaintEvent(QPaintEvent *event);
  int lineNumberAreaWidth();

protected:
  void resizeEvent(QResizeEvent *event) override;

private slots:
  void updateLineNumberAreaWidth(int newBlockCount);
  void highlightCurrentLine();
  void updateLineNumberArea(const QRect &rect, int dy);

private:
  QWidget *lineNumberArea;
};

#endif//QEDITIOR_EDITORWIDGET_H
