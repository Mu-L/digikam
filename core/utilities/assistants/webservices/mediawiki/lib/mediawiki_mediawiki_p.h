/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2011-03-22
 * Description : a MediaWiki C++ interface
 *
 * Copyright (C) 2011-2018 by Gilles Caulier <caulier dot gilles at gmail dot com>
 * Copyright (C) 2011      by Remi Benoit <r3m1 dot benoit at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#ifndef MEDIAWIKI_P_H
#define MEDIAWIKI_P_H

// Qt includes

#include <QString>
#include <QUrl>
#include <QNetworkAccessManager>

namespace mediawiki
{

class Q_DECL_HIDDEN MediaWiki::MediaWikiPrivate
{

public:

    MediaWikiPrivate(const QUrl& url, const QString& userAgent, QNetworkAccessManager* const manager)
        : url(url),
          userAgent(userAgent),
          manager(manager)
    {
    }

    ~MediaWikiPrivate()
    {
        delete manager;
    }

public:

    static const QString         POSTFIX_USER_AGENT;

    const QUrl                   url;
    const QString                userAgent;
    QNetworkAccessManager* const manager;
};

const QString MediaWiki::MediaWikiPrivate::POSTFIX_USER_AGENT = QString::fromUtf8("mediawiki-silk");

} // namespace mediawiki

#endif // MEDIAWIKI_P_H
