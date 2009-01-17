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


#include "analogpad.h"

static const int ANIM_TIMER = 50;
static const int EMIT_TIMER = 50;
static const QColor BACKGROUND_COLOR_1 = QColor(255, 255, 255);
static const QColor BACKGROUND_COLOR_2 = QColor(160, 160, 160);
static const QColor EFFECT_COLOR = QColor(100, 160, 225, 100);
static const qreal MAX_DIST = 0.5;
static const qreal PAD_SENSITIVITY_LIMIT = 0.4;

AnalogPad::AnalogPad(QWidget *parent)
    :QWidget(parent),
    m_Position(0, 0),
    m_PadPosition(0, 0),
    m_TouchPosition(0, 0),
    m_MouseDown(false),
    m_MouseClick(false),
    m_LimitReached(false),
    m_BackgroundColor1(BACKGROUND_COLOR_1),
    m_BackgroundColor2(BACKGROUND_COLOR_2),
    m_EffectColor(EFFECT_COLOR)
{
    // Start animation timer
    m_AnimTimer = new QTimer(this);
    connect(m_AnimTimer, SIGNAL(timeout()), this, SLOT(animationTick()));
    m_AnimTimer->start(ANIM_TIMER);

    // Start signal emit timer
    m_EmitTimer = new QTimer(this);
    connect(m_EmitTimer, SIGNAL(timeout()), this, SLOT(emitPadValue()));
    m_EmitTimer->start(EMIT_TIMER);

    // Click animation timeline
    m_ClickTimeLine = new QTimeLine(300, this);
    m_ClickTimeLine->setFrameRange(0, 20);
    connect(m_ClickTimeLine, SIGNAL(frameChanged(int)), this, SLOT(clickAnimationFrameChanged(int)));
}

AnalogPad::~AnalogPad()
{
}

// Set the animation frequency
void AnalogPad::setAnimationInterval(int mSec)
{
    m_AnimTimer->setInterval(mSec);
}

int AnalogPad::animationInterval()
{
    return m_AnimTimer->interval();
}

// Set the frequency of emitting "padValue" signal for widget user
void AnalogPad::setValueSignalInterval(int mSec)
{
    m_EmitTimer->setInterval(mSec);
}

int AnalogPad::valueSignalInterval()
{
    return m_EmitTimer->interval();
}

// Set the main background color
void AnalogPad::setBackgroundColor1(QColor color)
{
    m_BackgroundColor1 = color;
    paintBackground();
    update();
}

QColor AnalogPad::backgroundColor1()
{
    return m_BackgroundColor1;
}

// Set the secondary background color
void AnalogPad::setBackgroundColor2(QColor color)
{
    m_BackgroundColor2 = color;
    paintBackground();
    update();
}

QColor AnalogPad::backgroundColor2()
{
    return m_BackgroundColor2;
}

// Set the effect color
void AnalogPad::setEffectColor(QColor color)
{
    m_EffectColor = color;
}

QColor AnalogPad::effectColor()
{
    return m_EffectColor;
}

// Set the pad position
void AnalogPad::setValue(QPointF value, bool mouseDown)
{
    // Check boundaries
    qreal dist = sqrt(pow(value.x(), 2) + pow(value.y(), 2));			
    if(dist>MAX_DIST) 
    {
        value/=dist;
        value*=MAX_DIST;
    }
    m_PadPosition = QPointF((1 + value.x()) * m_Position.x(), 
                            (1 + value.y()) * m_Position.y());
    m_MouseDown = mouseDown;
    update();    
}

// Returns current pad position between -1..1, center being QPointF(0,0)
QPointF AnalogPad::value()
{
    return QPointF (m_PadPosition.x() / m_Position.x() - 1,
                    m_PadPosition.y() / m_Position.y() - 1);
}

// Convinience functions for getting the pad value
int AnalogPad::x()
{
    return value().x();
}

int AnalogPad::y()
{
    return value().y();
}

// Draw background pixmap separately, only updated after resize event and when
// m_LimitReached state is changed
void AnalogPad::paintBackground()
{
    m_BackgroundPixmap = QPixmap(this->size());
    m_BackgroundPixmap.fill(Qt::transparent);

    QPainter painter(&m_BackgroundPixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing);

    painter.setPen(m_BackgroundColor2);
    // Draw widget borders
    //painter.drawRect(this->rect().adjusted(0, 0, -1, -1));

    // Draw pad
    // Outer circle..
    QRadialGradient radialGrad(QPointF(m_Position.x(), m_Position.y()), this->height()*0.7, QPointF(this->width()*0.1,this->height()*0.1));
    int brightness = 95;
    if (m_LimitReached) brightness = 107;
    radialGrad.setColorAt(0, m_BackgroundColor1.lighter(brightness));
    radialGrad.setColorAt(1, m_BackgroundColor2.lighter(brightness));
    painter.setBrush(QBrush(radialGrad));    
    painter.drawEllipse(m_Position, this->width()*0.45, this->height()*0.45);
    
    // ..and inner circle
    radialGrad.setColorAt(0, m_BackgroundColor1);
    radialGrad.setColorAt(1, m_BackgroundColor2);
    painter.setBrush(QBrush(radialGrad));
    painter.drawEllipse(m_Position, this->width()*0.41, this->height()*0.41);

    // Draw pad arrows
    const qreal ARROW_SCALE = 0.2;
    const qreal ARROW_WIDTH = this->width()*ARROW_SCALE;
    const qreal ARROW_HEIGHT = this->height()*ARROW_SCALE;
    const qreal ARROW_DISTANCE = 0.38;
    const qreal ARROW_ANGLE = 60;

    QRectF rectangle(m_Position.x()-ARROW_WIDTH/2, m_Position.y()-ARROW_HEIGHT/2, ARROW_WIDTH, ARROW_HEIGHT);

    painter.setBrush(m_BackgroundColor1);
    painter.setOpacity(0.5);

    painter.drawPie(rectangle.adjusted(0, this->height()*ARROW_DISTANCE, 0, this->height()*ARROW_DISTANCE), (90-ARROW_ANGLE/2) * 16, ARROW_ANGLE * 16);
    painter.drawPie(rectangle.adjusted(this->width()*ARROW_DISTANCE, 0, this->width()*ARROW_DISTANCE, 0), (180-ARROW_ANGLE/2) * 16, ARROW_ANGLE * 16);
    painter.drawPie(rectangle.adjusted(0, this->height()*-ARROW_DISTANCE, 0, this->height()*-ARROW_DISTANCE), (270-ARROW_ANGLE/2) * 16, ARROW_ANGLE * 16);
    painter.drawPie(rectangle.adjusted(this->width()*-ARROW_DISTANCE, 0, this->width()*-ARROW_DISTANCE, 0), (360-ARROW_ANGLE/2) * 16, ARROW_ANGLE * 16);
    
    // Draw the cross in the middle
    painter.setOpacity(1);
    painter.drawLine(this->width()*0.48, this->height()/2, this->width()*0.52+1, this->height()/2);
    painter.drawLine(this->width()/2, this->height()*0.48, this->width()/2, this->height()*0.52+1);
}


// Event handlers
void AnalogPad::resizeEvent(QResizeEvent * event)
{
    // Set center position and pad to center
    m_Position = QPointF(qreal(event->size().width())/2, qreal(event->size().height())/2);
    m_PadPosition = m_Position;
    // Redraw background and update
    paintBackground();
    update();
}

void AnalogPad::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | 
                           QPainter::TextAntialiasing |
                           QPainter::HighQualityAntialiasing);

    // Draw background
    if (!m_BackgroundPixmap.isNull())
    {
        painter.drawPixmap(QPoint(0,0), m_BackgroundPixmap);
    }

    // Draw pad arrow effects
    const qreal ARROW_SCALE = 0.3;
    const qreal ARROW_WIDTH = this->width()*ARROW_SCALE;
    const qreal ARROW_HEIGHT = this->height()*ARROW_SCALE;
    const qreal ARROW_DISTANCE = 0.38;
    const qreal ARROW_ANGLE = 60;

    QRectF rectangle(m_Position.x()-ARROW_WIDTH/2, m_Position.y()-ARROW_HEIGHT/2, ARROW_WIDTH, ARROW_HEIGHT);

    painter.setBrush(m_EffectColor);
    painter.setOpacity(0.2);

    if (value().y() > 0.15) painter.drawPie(rectangle.adjusted(0, this->height()*ARROW_DISTANCE, 0, this->height()*ARROW_DISTANCE), (90-ARROW_ANGLE/2) * 16, ARROW_ANGLE * 16);
    if (value().x() > 0.15) painter.drawPie(rectangle.adjusted(this->width()*ARROW_DISTANCE, 0, this->width()*ARROW_DISTANCE, 0), (180-ARROW_ANGLE/2) * 16, ARROW_ANGLE * 16);
    if (value().y() < -0.15) painter.drawPie(rectangle.adjusted(0, this->height()*-ARROW_DISTANCE, 0, this->height()*-ARROW_DISTANCE), (270-ARROW_ANGLE/2) * 16, ARROW_ANGLE * 16);
    if (value().x() < -0.15) painter.drawPie(rectangle.adjusted(this->width()*-ARROW_DISTANCE, 0, this->width()*-ARROW_DISTANCE, 0), (360-ARROW_ANGLE/2) * 16, ARROW_ANGLE * 16);

    // Draw moving pad
    if(m_MouseDown) {
        painter.setOpacity(0.25);
    } else {
        painter.setOpacity(0.1);
    }
    painter.setBrush(m_BackgroundColor1.lighter(130));
    painter.drawEllipse(m_PadPosition, this->width()*0.2, this->height()*0.2);

    // Draw click animation
    if (m_ClickTimeLine->state() == QTimeLine::Running) 
    {
        painter.setBrush(m_EffectColor);
        painter.setOpacity(1.0 - (qreal)(m_ClickTimeLine->currentFrame() / (qreal)m_ClickTimeLine->endFrame()));
        painter.drawEllipse(m_Position, this->width()*0.2 + (m_ClickTimeLine->currentFrame()), this->height()*0.2 + (m_ClickTimeLine->currentFrame()));
    }
}

void AnalogPad::mousePressEvent(QMouseEvent *event)
{ 
    // Scale pad position to range [-1, 1]
    qreal xx = qreal(event->pos().x()) / m_Position.x() - 1;
    qreal yy = qreal(event->pos().y()) / m_Position.y() - 1;

    // Calculate the distance from the center
    qreal dist = sqrt(pow(xx, 2) + pow(yy, 2));

    // Return if outside the pad area
    if(dist>0.9) return;

    m_MouseDown = true;    
    m_TouchPosition = event->pos();
    
    // Calculate unit vectors
    xx /= dist;
    yy /= dist;
    
    // Check was the pad pressed close to the center
    if (dist > PAD_SENSITIVITY_LIMIT) 
    {
        // Check if the distance from the center exceeds maximum
        if(dist>MAX_DIST) 
        {
            dist = MAX_DIST;
            if(!m_LimitReached) 
            {
                m_LimitReached = true;
                paintBackground();
            }	
        } else {
            if(m_LimitReached) 
            {
                m_LimitReached = false;
                paintBackground();
            }
        }
        
        // Calculate new pad position
        QPoint newpos((xx*dist + 1) * m_Position.x(), (yy*dist + 1)*m_Position.y());
        setPadPosition(newpos);
        emitPadValue();       
    } else {
        m_MouseClick = true;
    }
    update();     
    emitValueChanged();    
}

void AnalogPad::mouseReleaseEvent(QMouseEvent *)
{
    // Return if mouse was not pressed inside the pad area
    if(!m_MouseDown) return;

    m_MouseDown = false;
    if(m_LimitReached) 
    {
        m_LimitReached = false;
        paintBackground();
    }    
    
    if (m_MouseClick)   
    {
        emit padClicked();
        startClickAnimation();
    }
    m_MouseClick = false;
    emitValueChanged();    
}

void AnalogPad::mouseMoveEvent(QMouseEvent *event)
{
    // Return if mouse was not pressed inside the pad area
    if(!m_MouseDown) return;

    if(m_MouseClick)
    {
        QPoint d = event->pos() - m_TouchPosition;
        if(d.manhattanLength()>4) m_MouseClick = false;
    }

    if (!m_MouseClick)
    {
        // Scale pad position to range [-1, 1]
        qreal xx = qreal(event->pos().x()) / m_Position.x() - 1;
        qreal yy = qreal(event->pos().y()) / m_Position.y() - 1;
        
        // Calculate the distance from the center
        qreal dist = sqrt(pow(xx, 2) + pow(yy, 2));
        
        // Calculate unit vectors
        xx/=dist;
        yy/=dist;

        // Check if the distance from the center exceeds maximum
        if(dist>MAX_DIST)
        {
            dist = MAX_DIST;
            if(!m_LimitReached)
            {
                m_LimitReached = true;
                paintBackground();
            }
        } else {
            if(m_LimitReached)
            {
                m_LimitReached = false;
                paintBackground();
            }
        }

        // Calculate new pad position
        QPoint newpos((xx*dist + 1) * m_Position.x(), (yy*dist + 1)*m_Position.y());
        setPadPosition(newpos);
        update();
        emitValueChanged();
    }	    
}

// Set the position of moving pad
void AnalogPad::setPadPosition(QPoint pos)
{
    m_PadPosition = pos;
}

// Emit padValue signal (repeatedly at a given interval)
void AnalogPad::emitPadValue()
{
    QPointF temp_padValue = value();
    
    if (qAbs(temp_padValue.x()) > 0.05 || qAbs(temp_padValue.y()) > 0.05)
    {
        emit padValue(temp_padValue);
    }
}

// Emit valueChanged signal (due to user action)
void AnalogPad::emitValueChanged()
{
    QPointF temp_padValue = value();
    emit valueChanged(temp_padValue, m_MouseDown);
}

void AnalogPad::startClickAnimation()
{
    if (m_ClickTimeLine->state() != QTimeLine::Running)
        m_ClickTimeLine->start();
}

void AnalogPad::animationTick()
{
    // Return if animation is not needed
    if ((qAbs(m_Position.x() - m_PadPosition.x()) < 1 && 
         qAbs(m_Position.y() - m_PadPosition.y()) < 1) || m_MouseDown)
        return;
    
    m_PadPosition = m_PadPosition - (m_PadPosition-m_Position)*(0.2);
    update();
}

// Update Animation 
void AnalogPad::clickAnimationFrameChanged(int)
{
    update();
}
