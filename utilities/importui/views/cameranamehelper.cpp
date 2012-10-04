/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2009-08-01
 * Description : camera name helper class
 *
 * Copyright (C) 2009-2012 by Andi Clemens <andi dot clemens at gmail dot com>
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

#include "cameranamehelper.h"

// Qt includes

#include <QAction>
#include <QRegExp>

// KDE includes

#include <klocale.h>

namespace
{
static const QString STR_AUTO_DETECTED("auto-detected");

static QRegExp REGEXP_CAMERA_NAME("^(.*)\\s*\\((.*)\\)\\s*$", Qt::CaseInsensitive);
static QRegExp REGEXP_MODES("^(ptp|normal|mtp)(\\s+mode)?$", Qt::CaseInsensitive);
static QRegExp REGEXP_AUTODETECTED(QString("(%1|, %1)").arg(STR_AUTO_DETECTED));
}

namespace Digikam
{

QString CameraNameHelper::createCameraName(const QString& vendor, const QString& product,
                                           const QString& mode,   bool autoDetected)
{
    if (vendor.isEmpty())
    {
        return QString();
    }

    QString tmp;
    QString _vendor  = vendor.simplified();
    QString _product = product.simplified();
    QString _mode    = mode.simplified().remove(QChar('(')).remove(QChar(')'));

    tmp = QString("%1 %2").arg(_vendor).arg(_product);

    if (!mode.isEmpty())
    {
        tmp.append(" (");
        tmp.append(_mode);
        tmp.append(autoDetected
                   ? QString(", %1)").arg(STR_AUTO_DETECTED)
                   : QString(')'));
    }
    else if (autoDetected)
    {
        tmp.append(QString(" (%1)").arg(STR_AUTO_DETECTED));
    }

    return tmp.simplified();
}

QString CameraNameHelper::formattedCameraName(const QString& name)
{
    return parseAndFormatCameraName(name, false, false);
}

QString CameraNameHelper::formattedFullCameraName(const QString& name, bool autoDetected)
{
    return parseAndFormatCameraName(name, true, autoDetected);
}

QString CameraNameHelper::parseAndFormatCameraName(const QString& cameraName,
                                                   bool parseMode, bool autoDetected)
{
    QString vendorAndProduct = extractCameraNameToken(cameraName, VendorAndProduct);

    if (vendorAndProduct.isEmpty())
    {
        return QString();
    }

    QString mode = parseMode
                   ? extractCameraNameToken(cameraName, Mode)
                   : QString();

    QString tmp = createCameraName(vendorAndProduct, QString(), mode, autoDetected);
    return tmp.isEmpty()
           ? cameraName.simplified()
           : tmp;
}

QString CameraNameHelper::extractCameraNameToken(const QString& cameraName, Token tokenID)
{
    REGEXP_CAMERA_NAME.setMinimal(true);
    REGEXP_MODES.setMinimal(true);
    REGEXP_AUTODETECTED.setMinimal(true);

    if (REGEXP_CAMERA_NAME.exactMatch(cameraName.simplified()))
    {
        QString vendorProduct  = REGEXP_CAMERA_NAME.cap(1).simplified();
        QString tmpMode        = REGEXP_CAMERA_NAME.cap(2).simplified();
        QString clearedTmpMode = tmpMode;
        QString mode;
        clearedTmpMode.remove(REGEXP_AUTODETECTED);

        if (!tmpMode.isEmpty() && clearedTmpMode.isEmpty())
        {
            mode = tmpMode;
        }
        else
        {
            mode = REGEXP_MODES.exactMatch(clearedTmpMode)
                   ? clearedTmpMode
                   : "";
        }

        if (tokenID == VendorAndProduct)
        {
            return mode.isEmpty()
                   ? cameraName.simplified()
                   : vendorProduct;
        }
        else
        {
            return mode;
        }
    }
    return (tokenID == VendorAndProduct)
           ? cameraName.simplified()
           : "";
}

bool CameraNameHelper::sameDevices(const QString& deviceA, const QString& deviceB)
{
    if (deviceA.isEmpty() || deviceB.isEmpty())
    {
        return false;
    }

    if (deviceA == deviceB)
    {
        return true;
    }

    // We need to parse the names a little bit. First check if the vendor and name match
    QString vendorAndProductA = extractCameraNameToken(deviceA, VendorAndProduct);
    QString vendorAndProductB = extractCameraNameToken(deviceB, VendorAndProduct);
    QString cameraNameA       = createCameraName(vendorAndProductA);
    QString cameraNameB       = createCameraName(vendorAndProductB);

    // try to clean up the string, if not possible, return false
    if (cameraNameA != cameraNameB)
    {
        return false;
    }

    // is the extracted mode known and equal?
    QString modeA       = extractCameraNameToken(deviceA, Mode);
    QString modeB       = extractCameraNameToken(deviceB, Mode);
    bool isModeAValid   = REGEXP_MODES.exactMatch(modeA);
    modeA               = isModeAValid ? REGEXP_MODES.cap(1).simplified().toLower() : "";
    bool isModeBValid   = REGEXP_MODES.exactMatch(modeB);
    modeB               = isModeBValid ? REGEXP_MODES.cap(1).simplified().toLower() : "";

    if ((isModeAValid != isModeBValid) || (modeA != modeB))
    {
        return false;
    }

    return true;
}

} // namespace Digikam
