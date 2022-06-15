//
// Created by PoteznyDominator on 22.05.2022.
//

#ifndef QEDITIOR_EDITORWIDGET_H
#define QEDITIOR_EDITORWIDGET_H

#include <QPlainTextEdit>

class EditorWidget : public QPlainTextEdit {
  Q_OBJECT
public:
  explicit EditorWidget(QWidget* parent = nullptr);

  void lineNumberAreaPaintEvent(QPaintEvent* event);
  int lineNumberAreaWidth();
  bool isChanged() const;
  void zoomIn(int range = 1);
  void zoomOut(int range = 1);

signals:
  void zoomInSignal();
  void zoomOutSignal();

protected:
  void resizeEvent(QResizeEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;

private:
  bool isChanged_;
  QWidget* lineNumberArea_;

  void updateLineNumberAreaWidth(int newBlockCount);
  void highlightCurrentLine();
  void updateLineNumberArea(const QRect& rect, int dy);
};

#endif//QEDITIOR_EDITORWIDGET_H
