/*
 * AnalogPad
 *
 * 5-way navigator Qt widget, with an analog twist
 *
 * Authors:
 * Kaj Grönholm <kaj.gronholm@nomovok.com>
 * Kim Grönholm <kim.gronholm@nomovok.com>
 *
 * Copyright (C) 2008 Authors @ QUIt Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * analogpad.cpp/h
 *
 * AnalogPad Qt widget. See README for more information
 */

#ifndef ANALOGPAD_H
#define ANALOGPAD_H

#include <QtGui>

class AnalogPad : public QWidget
{
    Q_OBJECT

public:
    AnalogPad(QWidget *parent = 0);
    ~AnalogPad();

    // Set the animation frequency
    void setAnimationInterval(int mSec);
    int animationInterval();

    // Set the frequency of emitting "padValue" signal for widget user
    void setValueSignalInterval(int mSec);
    int valueSignalInterval();

    // Set the main background color
    void setBackgroundColor1(QColor color);
    QColor backgroundColor1();

    // Set the secondary background color
    void setBackgroundColor2(QColor color);
    QColor backgroundColor2();

    // Set the effect color
    void setEffectColor(QColor color);
    QColor effectColor();

    // Get the pad value, with convinience functions
    QPointF value();
    int x();
    int y();

public slots:
    // Set the pad value
    void setValue(QPointF value, bool mouseDown);

signals:
    // Emited due to user actions
    void valueChanged(QPointF newValue, bool mouseDown);
    
    // Emited repeatedly at a given interval
    void padValue(QPointF curValue);
    
    // Emited when the center of the pad is clicked
    void padClicked();

protected:
    // Event handlers
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent * event);

private slots:
    void animationTick();
    void clickAnimationFrameChanged(int frame);
    void startClickAnimation();
    void emitPadValue();

private:
    void emitValueChanged();
    void setPadPosition(QPoint pos);
    void paintBackground();

    QPointF m_Position; // Center of the widget
    QPointF m_PadPosition; // Center of the pad
    QPoint m_TouchPosition;
    bool m_MouseDown;
    bool m_MouseClick;
    bool m_LimitReached;

    QTimer *m_AnimTimer;
    QTimer *m_EmitTimer;
    QTimeLine *m_ClickTimeLine;

    QColor m_BackgroundColor1;
    QColor m_BackgroundColor2;
    QColor m_EffectColor;

    QPixmap m_BackgroundPixmap;

};

#endif // ANALOGPAD_H
