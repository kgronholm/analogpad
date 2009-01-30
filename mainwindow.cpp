/*
 * AnalogPad
 *
 * 5-way navigator Qt widget, with an analog twist
 *
 * Authors:
 * Kaj Grönholm <kaj.gronholm@nomovok.com>
 * Kim Grönholm <kim.gronholm@nomovok.com>
 *
 * Copyright (C) 2009 Authors @ QUIt Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * mainwindow.cpp/h
 *
 * AnalogPad demo application
 *
 * Contains 3 AnalogPad widgets, with different settings, signalled to work together.
 * Widgets can be used to control QWebView browser widget
 * NOTE: Internet connection required to get browser into "google.com"
 */

#include <QWebFrame>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "analogpad.h"

static const int SCROLL_SPEED = 20;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);

    // analogPad1 showing default settings

    // Setup analogPad2
    ui->analogPad2->setBackgroundColor1(QColor(228, 223, 165));
    ui->analogPad2->setBackgroundColor2(QColor(174, 153, 78));
    ui->analogPad2->setEffectColor(QColor(255, 255, 255, 120));
    ui->analogPad2->setValueSignalInterval(50);
    ui->analogPad2->setAnimationInterval(50);

    // Setup analogPad3
    ui->analogPad3->setBackgroundColor1(QColor(255, 213, 213));
    ui->analogPad3->setBackgroundColor2(QColor(255, 128, 128));
    ui->analogPad3->setEffectColor(QColor(255, 128, 128, 120));

    // Connect analogPad1 to webView
    connect(ui->analogPad1, SIGNAL(padValue(QPointF)), this, SLOT(updateView(QPointF)));

    // connect all pads together
    connect(ui->analogPad1, SIGNAL(valueChanged(QPointF, bool)), ui->analogPad2, SLOT(setValue(QPointF, bool)));
    connect(ui->analogPad1, SIGNAL(valueChanged(QPointF, bool)), ui->analogPad3, SLOT(setValue(QPointF, bool)));
    connect(ui->analogPad2, SIGNAL(valueChanged(QPointF, bool)), ui->analogPad1, SLOT(setValue(QPointF, bool)));
    connect(ui->analogPad2, SIGNAL(valueChanged(QPointF, bool)), ui->analogPad3, SLOT(setValue(QPointF, bool)));
    connect(ui->analogPad3, SIGNAL(valueChanged(QPointF, bool)), ui->analogPad1, SLOT(setValue(QPointF, bool)));
    connect(ui->analogPad3, SIGNAL(valueChanged(QPointF, bool)), ui->analogPad2, SLOT(setValue(QPointF, bool)));

    connect(ui->analogPad1, SIGNAL(padClicked()), ui->analogPad2, SLOT(startClickAnimation()));
    connect(ui->analogPad1, SIGNAL(padClicked()), ui->analogPad3, SLOT(startClickAnimation()));
    connect(ui->analogPad2, SIGNAL(padClicked()), ui->analogPad1, SLOT(startClickAnimation()));
    connect(ui->analogPad2, SIGNAL(padClicked()), ui->analogPad3, SLOT(startClickAnimation()));
    connect(ui->analogPad3, SIGNAL(padClicked()), ui->analogPad1, SLOT(startClickAnimation()));
    connect(ui->analogPad3, SIGNAL(padClicked()), ui->analogPad2, SLOT(startClickAnimation()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Demonstate by scrolling WebView
void MainWindow::updateView(QPointF newValue)
{
    QWebPage *page = ui->webView->page();
    QWebFrame *frame = page->mainFrame();
    frame->setScrollBarValue(Qt::Horizontal, frame->scrollBarValue(Qt::Horizontal) + newValue.x() * SCROLL_SPEED);
    frame->setScrollBarValue(Qt::Vertical, frame->scrollBarValue(Qt::Vertical) + newValue.y() * SCROLL_SPEED);
}
