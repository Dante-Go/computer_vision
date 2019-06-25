#ifndef QENHANCEDGRAPHICSVIEW_H
#define QENHANCEDGRAPHICSVIEW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QtMath>
#include <QMenu>
#include <QGraphicsItem>
#include <QGraphicsEffect>


class QEnhancedGraphicsView : public QGraphicsView
{
  Q_OBJECT
public:
  explicit QEnhancedGraphicsView(QWidget *parent = nullptr);
protected:
  void wheelEvent(QWheelEvent *event);
private:
  QPointF sceneMousePos;
};

#endif // QENHANCEDGRAPHICSVIEW_H
