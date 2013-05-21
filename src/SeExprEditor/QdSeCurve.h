/*
* (c) Disney Enterprises, Inc.  All rights reserved.
*
* This file is licensed under the terms of the Microsoft Public License (MS-PL)
* as defined at: http://opensource.org/licenses/MS-PL.
*
* A complete copy of this license is included in this distribution as the file
* LICENSE.
*
* @file QdSeCurve.h
* @brief Contains PyQt4 Ramp Widget to emulate Maya's ramp widget
* @author Arthur Shek
* @version ashek     05/04/09  Initial Version
*/
#ifndef _QdSeCurve_h_
#define _QdSeCurve_h_


#include <vector>

#include <QtCore/QObject>
#include <QtGui/QComboBox>
#include <QtGui/QGraphicsPolygonItem>
#include <QtGui/QGraphicsView>
#include <QtGui/QLineEdit>

#include <SeCurve.h>

/*
  This class overrides QGraphicsView so we can get resize events
*/
class CurveGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    CurveGraphicsView() {
        setTransformationAnchor(QGraphicsView::NoAnchor);
        setResizeAnchor(QGraphicsView::NoAnchor);
    }
    ~CurveGraphicsView() {}

    virtual void resizeEvent(QResizeEvent *event);

signals:
    void resizeSignal(int width, int height);
};

/*
  This class overrides QGraphicsScene so we can handle mouse
  press, drag and keyboard events
*/
class CurveScene : public QGraphicsScene
{
    Q_OBJECT

    typedef SeExpr::SeCurve<double> T_CURVE;
    typedef T_CURVE::InterpType T_INTERP ;
public:
    CurveScene();
    ~CurveScene();

    void addPoint(double x, double y, const T_INTERP interp, const bool select=true);

    void removePoint(const int index);
    void removeAll();

    virtual void keyPressEvent(QKeyEvent *event);

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

    void drawRect();

    void drawPoly();

    void drawPoints();

    void emitCurveChanged();

    void rebuildCurve();

    std::vector<T_CURVE::CV> _cvs; // unsorted cvs

    friend class QdSeCurve;

private:
    T_CURVE* _curve;
public slots:
    void interpChanged(const int interp);
    void selPosChanged(double pos);
    void selValChanged(double val);
    void resize(const int width, const int height);

signals:
    void cvSelected(double x, double y, T_INTERP interp);
    void curveChanged();

private:

    int _width;
    int _height;
    T_INTERP _interp;
    std::vector<QGraphicsEllipseItem *> _circleObjects;
    int _selectedItem;
    QGraphicsPolygonItem *_curvePoly;
    QGraphicsRectItem *_baseRect;
    bool _lmb;
};

class QdSeCurve : public QWidget
{
    Q_OBJECT 

    typedef SeExpr::SeCurve<double> T_CURVE;
    typedef T_CURVE::InterpType T_INTERP ;

public:
    QdSeCurve(QWidget* parent = 0, QString pLabel = "", QString vLabel = "", QString iLabel = "",
        bool expandable=true);
    ~QdSeCurve() {}

    // Convenience Functions
    void addPoint(const double x, const double y, const T_INTERP interp, bool select=false);

    CurveScene *_scene;

public slots:
    void cvSelectedSlot(double pos, double val, T_INTERP interp);
    void selPosChanged();
    void selValChanged();
    void openDetail();

signals:
    void selPosChangedSignal(double pos);
    void selValChangedSignal(double val);

private:
    QLineEdit *_selPosEdit;
    QLineEdit *_selValEdit;
    QComboBox *_interpComboBox;
};
#endif