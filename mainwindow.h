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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

namespace Ui
{
    class MainWindowClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateView(QPointF newValue);

private:
    Ui::MainWindowClass *ui;
};

#endif // MAINWINDOW_H
