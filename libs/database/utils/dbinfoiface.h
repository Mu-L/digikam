/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2017-05-06
 * Description : interface to database informations for shared tools.
 *
 * Copyright (C) 2017 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef DB_INFO_IFACE_H
#define DB_INFO_IFACE_H

// Local includes

#include "dinfointerface.h"
#include "digikam_export.h"

namespace Digikam
{

class DIGIKAM_EXPORT DBInfoIface : public DInfoInterface
{

public:

    explicit DBInfoIface(QObject* const parent);
    ~DBInfoIface();

    QList<QUrl> currentSelection()  const;
    QList<QUrl> currentAlbum()      const;
    QList<QUrl> allAlbums()         const;

    DInfoMap albumInfo(const QUrl&) const;
    DInfoMap itemInfo(const QUrl&)  const;

private:

    class Private;
    Private* const d;
};

}  // namespace Digikam

#endif  // DB_INFO_IFACE_H
