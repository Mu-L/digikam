/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2010-06-29
 * Description : Pressable Button class using QGraphicsItem
 *               based on Frederico Duarte implementation.
 *
 * Copyright (C) 2010-2019 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef DIGIKAM_DEMO_BUTTON_H
#define DIGIKAM_DEMO_BUTTON_H

// Qt includes

#include <QObject>
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

namespace Digikam
{

class Button : public QObject,
               public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:

    explicit Button(QGraphicsItem* const parent = 0);
    explicit Button(const QString& normal, const QString& pressed = QString(), QGraphicsItem* const parent = 0);
    Button(const QPixmap& normal, const QPixmap& pressed, QGraphicsItem* const parent = 0);
    ~Button();

    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
    void setPixmap(const QString& normal, const QString& pressed = QString());
    void setPixmap(const QPixmap& normal, const QPixmap& pressed);

Q_SIGNALS:

    void clicked();

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent*);
    void mouseMoveEvent(QGraphicsSceneMouseEvent*);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*);

private:

    class Private;
    Private* const d;
};

} // namespace Digikam

#endif // DIGIKAM_DEMO_BUTTON_H
