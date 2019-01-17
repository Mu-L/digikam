/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2008-06-17
 * Description : Find Duplicates tree-view search album item.
 *
 * Copyright (C) 2008-2019 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef DIGIKAM_FIND_DUPLICATES_ALBUM_ITEM_H
#define DIGIKAM_FIND_DUPLICATES_ALBUM_ITEM_H

// Qt includes

#include <QTreeWidget>
#include <QUrl>

// Local includes

#include "iteminfo.h"
#include "thumbnailloadthread.h"

namespace Digikam
{

class SAlbum;

class FindDuplicatesAlbumItem : public QTreeWidgetItem
{

public:

    enum Column
    {
      REFERENCE_IMAGE = 0,
      RESULT_COUNT    = 1,
      AVG_SIMILARITY  = 2
    };

public:

    explicit FindDuplicatesAlbumItem(QTreeWidget* const parent, SAlbum* const album);
    virtual ~FindDuplicatesAlbumItem();

    bool hasValidThumbnail() const;

    /**
     * Calculates the duplicates count and average similarity.
     **/
    void calculateInfos(const QList<qlonglong>& deletedImages = QList<qlonglong>());

    /**
     * Returns the item count.
     **/
    int itemCount()  const;

    SAlbum* album()  const;
    QUrl    refUrl() const;

    void setThumb(const QPixmap& pix, bool hasThumb = true);

    bool operator<(const QTreeWidgetItem& other) const;

private:

    class Private;
    Private* const d;
};

} // namespace Digikam

#endif // DIGIKAM_FIND_DUPLICATES_ALBUM_ITEM_H
