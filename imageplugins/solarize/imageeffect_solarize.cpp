/* ============================================================
 * File  : imageeffect_solarize.cpp
 * Author: Renchi Raju <renchi@pooh.tam.uiuc.edu>
 *         Gilles Caulier <caulier dot gilles at kdemail dot net>
 * Date  : 2004-02-14
 * Description : a digiKam image plugin for to solarize
 *               an image.
 *
 * Copyright 2004-2005 by Renchi Raju
 * Copyright 2006 by Gilles Caulier
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

#include <qvgroupbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qwhatsthis.h>
#include <qlayout.h>
#include <qframe.h>

// KDE includes.

#include <knuminput.h>
#include <klocale.h>
#include <kcursor.h>
#include <kaboutdata.h>
#include <khelpmenu.h>
#include <kiconloader.h>
#include <kapplication.h>
#include <kpopupmenu.h>
#include <kstandarddirs.h>

// Digikam includes.

#include <digikamheaders.h>

// Local includes.

#include "version.h"
#include "bannerwidget.h"
#include "imageeffect_solarize.h"

namespace DigikamSolarizeImagesPlugin
{

ImageEffect_Solarize::ImageEffect_Solarize(QWidget* parent, QString title, QFrame* banner)
                    : Digikam::ImageDlgBase(parent, title, "solarizeimage", false, false, banner)
{
    // About data and help button.

    KAboutData *about = new KAboutData("digikamimageplugins",
                            I18N_NOOP("Solarize a Photograph"),
                            digikamimageplugins_version,
                            I18N_NOOP("A solarize image plugin for digiKam."),
                            KAboutData::License_GPL,
                            "(c) 2004-2005, Renchi Raju\n(c) 2006, Gilles Caulier",
                            0,
                            "http://extragear.kde.org/apps/digikamimageplugins");

    about->addAuthor("Renchi Raju", I18N_NOOP("Original Author"),
                     "renchi@pooh.tam.uiuc.edu");
    
    about->addAuthor("Caulier Gilles", I18N_NOOP("Maintainer"),
                     "caulier dot gilles at kdemail dot net");

    setAboutData(about);

    // -------------------------------------------------------------

    m_previewWidget = new Digikam::ImageWidget("solarizeimage Tool Dialog", plainPage(),
                          i18n("<p>This is the solarize effect preview"));

    setPreviewAreaWidget(m_previewWidget); 

    // -------------------------------------------------------------

    QWidget *gboxSettings = new QWidget(plainPage());
    QGridLayout* grid = new QGridLayout( gboxSettings, 2, 1, marginHint(), spacingHint());

    QLabel *label = new QLabel(i18n("Intensity:"), gboxSettings);
    m_numInput = new KDoubleNumInput(gboxSettings);
    m_numInput->setPrecision(1);
    m_numInput->setRange(0.0, 100.0, 0.1, true);
    grid->addMultiCellWidget(label, 0, 0, 0, 0);
    grid->addMultiCellWidget(m_numInput, 1, 1, 0, 0);

    grid->setRowStretch(2, 10);
    setUserAreaWidget(gboxSettings);

    // -------------------------------------------------------------

    connect(m_numInput, SIGNAL(valueChanged (double)),
            this, SLOT(slotTimer()));

    connect(m_previewWidget, SIGNAL(signalResized()),
            this, SLOT(slotEffect()));
}

ImageEffect_Solarize::~ImageEffect_Solarize()
{
    delete m_numInput;
    delete m_previewWidget;
}

void ImageEffect_Solarize::slotEffect()
{
    Digikam::ImageIface* iface = m_previewWidget->imageIface();
    uchar *data                = iface->getPreviewImage();
    int w                      = iface->previewWidth();
    int h                      = iface->previewHeight();
    bool sb                    = iface->previewSixteenBit();

    solarize(m_numInput->value(), data, w, h, sb);

    iface->putPreviewImage(data);
    delete [] data;
    m_previewWidget->updatePreview();
}

void ImageEffect_Solarize::slotOk()
{
    kapp->setOverrideCursor( KCursor::waitCursor() );
    Digikam::ImageIface* iface = m_previewWidget->imageIface();
    uchar *data                = iface->getOriginalImage();
    int w                      = iface->originalWidth();
    int h                      = iface->originalHeight();
    bool sb                    = iface->originalSixteenBit();

    if (data)
    {
       solarize(m_numInput->value(), data, w, h, sb);
       iface->putOriginalImage(i18n("Solarize"), data);
       delete [] data;
    }

    kapp->restoreOverrideCursor();
    accept();
}

void ImageEffect_Solarize::solarize(double factor, uchar *data, int w, int h, bool sb)
{
    bool stretch = true;

    if (!sb)        // 8 bits image.
    {
        uint threshold = (uint)((100-factor)*(255+1)/100);
        threshold      = QMAX(1, threshold);
        uchar *ptr = data;
        uchar  a, r, g, b;

        for (int x=0 ; x < w*h ; x++)
        {
            b = ptr[0];
            g = ptr[1];
            r = ptr[2];
            a = ptr[3];

            if (stretch) 
            {
                r = (r > threshold) ? (255-r)*255/(255-threshold) : r*255/threshold;
                g = (g > threshold) ? (255-g)*255/(255-threshold) : g*255/threshold;
                b = (b > threshold) ? (255-b)*255/(255-threshold) : b*255/threshold;
            }
            else 
            {
                if (r > threshold)
                    r = (255-r);
                if (g > threshold)
                    g = (255-g);
                if (b > threshold)
                    b = (255-b);
            }

            ptr[0] = b;
            ptr[1] = g;
            ptr[2] = r;
            ptr[3] = a;

            ptr += 4;
        }
    }
    else                            // 16 bits image.
    {
        uint threshold = (uint)((100-factor)*(65535+1)/100);
        threshold      = QMAX(1, threshold);
        unsigned short *ptr = (unsigned short *)data;
        unsigned short  a, r, g, b;

        for (int x=0 ; x < w*h ; x++)
        {
            b = ptr[0];
            g = ptr[1];
            r = ptr[2];
            a = ptr[3];

            if (stretch) 
            {
                r = (r > threshold) ? (65535-r)*65535/(65535-threshold) : r*65535/threshold;
                g = (g > threshold) ? (65535-g)*65535/(65535-threshold) : g*65535/threshold;
                b = (b > threshold) ? (65535-b)*65535/(65535-threshold) : b*65535/threshold;
            }
            else 
            {
                if (r > threshold)
                    r = (65535-r);
                if (g > threshold)
                    g = (65535-g);
                if (b > threshold)
                    b = (65535-b);
            }

            ptr[0] = b;
            ptr[1] = g;
            ptr[2] = r;
            ptr[3] = a;

            ptr += 4;
        }
    }
}

}  // NameSpace DigikamSolarizeImagesPlugin

#include "imageeffect_solarize.moc"
