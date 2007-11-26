/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2007-11-25
 * Description : a bar used to search a string.
 * 
 * Copyright (C) 2007 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

// Qt includes.

#include <qcolor.h>
#include <qpalette.h>
#include <qpainter.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtoolbutton.h>

// KDE includes.

#include <kapplication.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kdialogbase.h>

// Local includes

#include "searchtextbar.h"
#include "searchtextbar.moc"

namespace Digikam
{

class DLineEditPriv
{
public:

    DLineEditPriv()
    {
        drawMsg = true;
    }

    bool    drawMsg;
    
    QString message;
};

DLineEdit::DLineEdit(const QString &msg, QWidget *parent)
         : KLineEdit(parent)
{
    d = new DLineEditPriv;
    setClickMessage(msg);
}

DLineEdit::~DLineEdit()
{
    delete d;
}

void DLineEdit::setClickMessage(const QString &msg)
{
    d->message = msg;
    repaint();
}

void DLineEdit::setDrawMessage(bool draw)
{
    d->drawMsg = draw;
    repaint();
}

bool DLineEdit::drawMessage() const
{
    return d->drawMsg;
}

void DLineEdit::setText(const QString &txt)
{
    d->drawMsg = txt.isEmpty();
    repaint();
    KLineEdit::setText(txt);
}

void DLineEdit::drawContents(QPainter *p)
{
    KLineEdit::drawContents(p);

    if (d->drawMsg && !hasFocus())
    {
        QPen tmp = p->pen();
        p->setPen(palette().color( QPalette::Disabled, QColorGroup::Text));
        QRect cr = contentsRect();

        // Add two pixel margin on the left side
        cr.rLeft() += 3;
        p->drawText( cr, AlignAuto | AlignVCenter, d->message );
        p->setPen( tmp );
    }
}

void DLineEdit::dropEvent(QDropEvent *e)
{
    d->drawMsg = false;
    KLineEdit::dropEvent(e);
}

void DLineEdit::focusInEvent(QFocusEvent *e)
{
    if (d->drawMsg)
    {
        d->drawMsg = false;
        repaint();
    }
    QLineEdit::focusInEvent(e);
}

void DLineEdit::focusOutEvent(QFocusEvent *e)
{
    if (text().isEmpty())
    {
        d->drawMsg = true;
        repaint();
    }
    QLineEdit::focusOutEvent(e);
}

// ---------------------------------------------------------------------

class SearchTextBarPriv
{
public:

    SearchTextBarPriv()
    {
        searchEdit  = 0;
        clearButton = 0;
    }

    QToolButton *clearButton;

    KLineEdit   *searchEdit;
};

SearchTextBar::SearchTextBar(QWidget *parent)
             : QWidget(parent, 0, Qt::WDestructiveClose)
{
    d = new SearchTextBarPriv;
    setFocusPolicy(QWidget::NoFocus);

    QHBoxLayout *hlay = new QHBoxLayout(this);

    d->clearButton = new QToolButton(this);
    d->clearButton->setEnabled(false);
    d->clearButton->setAutoRaise(true);
    d->clearButton->setIconSet(kapp->iconLoader()->loadIcon("clear_left",
                               KIcon::Toolbar, KIcon::SizeSmall));

    d->searchEdit  = new DLineEdit(i18n("Search..."), this);
    d->searchEdit->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum));

    hlay->setSpacing(0);
    hlay->setMargin(0);
    hlay->addWidget(d->searchEdit);
    hlay->addWidget(d->clearButton);

    connect(d->clearButton, SIGNAL(clicked()),
            d->searchEdit, SLOT(clear()));

    connect(d->searchEdit, SIGNAL(textChanged(const QString&)),
            this, SLOT(slotTextChanged(const QString&)));
}

SearchTextBar::~SearchTextBar()
{
    delete d;
}

void SearchTextBar::setText(const QString& text)
{
    d->searchEdit->setText(text);
}

QString SearchTextBar::text() const
{
    return d->searchEdit->text();
}

void SearchTextBar::slotTextChanged(const QString& text)
{
    d->clearButton->setEnabled(text.isEmpty() ? false : true);
        
    emit signalTextChanged(text);
}

void SearchTextBar::slotSearchResult(bool match)
{
    if (d->searchEdit->text().isEmpty())
    {
        d->searchEdit->unsetPalette();
        return;
    }

    QPalette pal = d->searchEdit->palette();
    pal.setColor(QPalette::Active, QColorGroup::Base,
                 match ?  QColor(200, 255, 200) :
                 QColor(255, 200, 200));
    d->searchEdit->setPalette(pal);
}

}  // namespace Digikam
