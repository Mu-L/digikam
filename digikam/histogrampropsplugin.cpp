/* ============================================================
 * File  : histogramproposplugin.h
 * Author: Gilles Caulier <caulier dot gilles at free.fr>
 * Date  : 2004-07-28
 * Description : an image histogram viewer dialog.
 * 
 * Copyright 2004 by Gilles Caulier
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
 
#include <qlayout.h>
#include <qcolor.h>
#include <qframe.h>
#include <qgroupbox.h>
#include <qvgroupbox.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qwhatsthis.h>

// KDE includes.

#include <klocale.h>
#include <kselect.h>
#include <kdialogbase.h>

// Local includes.

#include <imagehistogram.h>
#include <histogramwidget.h> 
#include "histogrampropsplugin.h"

// This widget is created for drawing a color gradient without arrow.
class GradientWidget : public KGradientSelector
{
public:

    GradientWidget(Orientation o, QWidget *parent=0)
                      : KGradientSelector (o, parent)
    {}      

protected:
    
    virtual void drawArrow( QPainter *painter, bool show, const QPoint &pos )
    {
    // Do nothing !!! We won't arrow...
    }
};

HistogramPropsPlugin::HistogramPropsPlugin( KPropertiesDialog *propsDlg, QString fileName )
                    : KPropsDlgPlugin(propsDlg)
{    
    if (m_image.load(fileName) == true)
       {
       if(m_image.depth() < 32)                 // we works always with 32bpp.
          m_image = m_image.convertDepth(32);
       
       m_image.setAlphaBuffer(true);
       setupGui(propsDlg,
                (uint *)m_image.bits(),
                m_image.width(), 
                m_image.height());
       }
}

HistogramPropsPlugin::~HistogramPropsPlugin()
{
    delete m_histogramWidget;
    delete m_hGradient;
}

void HistogramPropsPlugin::setupGui(KPropertiesDialog *dialog, uint *imageData, uint width, uint height)
{
    QFrame *page = dialog->addPage( i18n("&Histogram"));
   
    QVBoxLayout *topLayout = new QVBoxLayout( page, 0 );

    // -------------------------------------------------------------
                                              
    QHBoxLayout *hlay = new QHBoxLayout(topLayout);
    QLabel *label1 = new QLabel(i18n("Channel :"), page);
    label1->setAlignment ( Qt::AlignRight | Qt::AlignVCenter );
    m_channelCB = new QComboBox( false, page );
    m_channelCB->insertItem( i18n("Luminosity") );
    m_channelCB->insertItem( i18n("Red") );
    m_channelCB->insertItem( i18n("Green") );
    m_channelCB->insertItem( i18n("Blue") );
    m_channelCB->insertItem( i18n("Alpha") );
    m_channelCB->setCurrentText( i18n("Luminosity") );
    QWhatsThis::add( m_channelCB, i18n("<p>Select here the histogram channel to display"));
    
    QLabel *label2 = new QLabel(i18n("Scale :"), page);
    label2->setAlignment ( Qt::AlignRight | Qt::AlignVCenter);
    m_scaleCB = new QComboBox( false, page );
    m_scaleCB->insertItem( i18n("Linear") );
    m_scaleCB->insertItem( i18n("Logarithmic") );
    m_scaleCB->setCurrentText( i18n("Logarithmic") );
    QWhatsThis::add( m_scaleCB, i18n("<p>Select here the histogram scale"));
    
    hlay->addWidget(label1);
    hlay->addWidget(m_channelCB);
    hlay->addWidget(label2);
    hlay->addWidget(m_scaleCB);
    
    // -------------------------------------------------------------
    
    QFrame *frame = new QFrame( page );
    frame->setFrameStyle(QFrame::Panel|QFrame::Sunken);
    QVBoxLayout* l = new QVBoxLayout(frame, 5, 0);
    
    m_histogramWidget = new Digikam::HistogramWidget(256, 140, 
                                                     imageData,
                                                     width,
                                                     height,
                                                     frame);
    QWhatsThis::add( m_histogramWidget, i18n("<p>This is the histogram of the selected image channel"));
    l->addWidget(m_histogramWidget, 0);
        
    m_hGradient = new GradientWidget( KSelector::Horizontal, page );
    m_hGradient->setColors( QColor( "black" ), QColor( "white" ) );
    m_hGradient->setFixedHeight( 20 );
    topLayout->addWidget(frame);
    topLayout->addWidget(m_hGradient);

    // -------------------------------------------------------------

    QHBoxLayout *hlay2 = new QHBoxLayout(topLayout);
    QLabel *label3 = new QLabel(i18n("Intensity range :"), page);
    label3->setAlignment ( Qt::AlignRight | Qt::AlignVCenter);
    m_minInterv = new QSpinBox(0, 255, 1, page);
    m_maxInterv = new QSpinBox(0, 255, 1, page);
    m_maxInterv->setValue(255);
    hlay2->addWidget(label3);
    hlay2->addWidget(m_minInterv);
    hlay2->addWidget(m_maxInterv);
    
    // -------------------------------------------------------------
    
    QGroupBox *gbox = new QGroupBox(4, Qt::Horizontal, i18n("Informations"), page);
    
    QLabel *label4 = new QLabel(i18n("Mean :"), gbox);
    label4->setAlignment ( Qt::AlignRight | Qt::AlignVCenter);
    m_labelMeanValue = new QLabel(gbox);
    m_labelMeanValue->setAlignment ( Qt::AlignLeft | Qt::AlignVCenter);
    
    QLabel *label5 = new QLabel(i18n("Pixels :"), gbox);
    label5->setAlignment ( Qt::AlignRight | Qt::AlignVCenter);
    m_labelPixelsValue = new QLabel(gbox);
    m_labelMeanValue->setAlignment ( Qt::AlignLeft | Qt::AlignVCenter);
    
    QLabel *label6 = new QLabel(i18n("Std Dev :"), gbox);
    label6->setAlignment ( Qt::AlignRight | Qt::AlignVCenter);
    m_labelStdDevValue = new QLabel(gbox);
    m_labelStdDevValue->setAlignment ( Qt::AlignLeft | Qt::AlignVCenter);

    QLabel *label7 = new QLabel(i18n("Count :"), gbox);
    label7->setAlignment ( Qt::AlignRight | Qt::AlignVCenter);
    m_labelCountValue = new QLabel(gbox);
    m_labelCountValue->setAlignment ( Qt::AlignLeft | Qt::AlignVCenter);
    
    QLabel *label8 = new QLabel(i18n("Median :"), gbox);
    label8->setAlignment ( Qt::AlignRight | Qt::AlignVCenter);
    m_labelMedianValue = new QLabel(gbox);
    m_labelMedianValue->setAlignment ( Qt::AlignLeft | Qt::AlignVCenter);
    
    QLabel *label9 = new QLabel(i18n("Percentile :"), gbox);
    label9->setAlignment ( Qt::AlignRight | Qt::AlignVCenter);
    m_labelPercentileValue = new QLabel(gbox);
    m_labelPercentileValue->setAlignment ( Qt::AlignLeft | Qt::AlignVCenter);
    
    topLayout->addWidget(gbox);
    
    // -------------------------------------------------------------
    
    connect(m_channelCB, SIGNAL(activated(int)),
            this, SLOT(slotChannelChanged(int)));
    
    connect(m_scaleCB, SIGNAL(activated(int)),
            this, SLOT(slotScaleChanged(int)));
 
    connect(m_histogramWidget, SIGNAL(signalMousePressed( int )),
            this, SLOT(slotUpdateMinInterv(int)));
       
    connect(m_histogramWidget, SIGNAL(signalMouseReleased( int )),
            this, SLOT(slotUpdateMaxInterv(int)));

    connect(m_minInterv, SIGNAL(valueChanged (int)),
            m_histogramWidget, SLOT(slotMinValueChanged(int)));

    connect(m_minInterv, SIGNAL(valueChanged (int)),
            this, SLOT(slotIntervChanged(int)));

    connect(m_maxInterv, SIGNAL(valueChanged (int)),
            m_histogramWidget, SLOT(slotMaxValueChanged(int)));

    connect(m_maxInterv, SIGNAL(valueChanged (int)),
            this, SLOT(slotIntervChanged(int)));
}

void HistogramPropsPlugin::slotChannelChanged(int channel)
{
    switch(channel)
       {
       case 1:           // Red.
          m_histogramWidget->m_channelType = Digikam::HistogramWidget::RedChannelHistogram;
          m_hGradient->setColors( QColor( "black" ), QColor( "red" ) );
          break;
       
       case 2:           // Green.
          m_histogramWidget->m_channelType = Digikam::HistogramWidget::GreenChannelHistogram;
          m_hGradient->setColors( QColor( "black" ), QColor( "green" ) );
          break;
          
       case 3:           // Blue.
          m_histogramWidget->m_channelType = Digikam::HistogramWidget::BlueChannelHistogram;
          m_hGradient->setColors( QColor( "black" ), QColor( "blue" ) );
          break;

       case 4:           // Alpha.
          m_histogramWidget->m_channelType = Digikam::HistogramWidget::AlphaChannelHistogram;
          m_hGradient->setColors( QColor( "black" ), QColor( "white" ) );
          break;
                    
       default:          // Luminosity.
          m_histogramWidget->m_channelType = Digikam::HistogramWidget::ValueHistogram;
          m_hGradient->setColors( QColor( "black" ), QColor( "white" ) );
          break;
       }
   
    m_histogramWidget->repaint(false);
    updateInformations();
}

void HistogramPropsPlugin::slotScaleChanged(int scale)
{
    switch(scale)
       {
       case 1:           // Log.
          m_histogramWidget->m_scaleType = Digikam::HistogramWidget::LogScaleHistogram;
          break;
          
       default:          // Lin.
          m_histogramWidget->m_scaleType = Digikam::HistogramWidget::LinScaleHistogram;
          break;
       }
   
    m_histogramWidget->repaint(false);
}

void HistogramPropsPlugin::slotUpdateMinInterv(int min)
{
    m_minInterv->setValue(min);
}


void HistogramPropsPlugin::slotUpdateMaxInterv(int max)
{
    m_maxInterv->setValue(max);
    updateInformations();
}

void HistogramPropsPlugin::slotIntervChanged(int)
{
    m_maxInterv->setMinValue(m_minInterv->value());
    m_minInterv->setMaxValue(m_maxInterv->value());
    updateInformations();
}

void HistogramPropsPlugin::updateInformations()
{
    QString value;
    int min = m_minInterv->value();
    int max = m_maxInterv->value();
    int channel = m_channelCB->currentItem();
    
    double mean = m_histogramWidget->m_imageHistogram->getMean(channel, min, max);
    m_labelMeanValue->setText(value.sprintf("%3.1f", mean));
    
    double pixels = m_histogramWidget->m_imageHistogram->getPixels();
    m_labelPixelsValue->setText(value.sprintf("%8d", (int)pixels));
    
    double stddev = m_histogramWidget->m_imageHistogram->getStdDev(channel, min, max);
    m_labelStdDevValue->setText(value.sprintf("%3.1f", stddev));
      
    double counts = m_histogramWidget->m_imageHistogram->getCount(channel, min, max);
    m_labelCountValue->setText(value.sprintf("%8d", (int)counts));
    
    double median = m_histogramWidget->m_imageHistogram->getMedian(channel, min, max);
    m_labelMedianValue->setText(value.sprintf("%3.1f", median));

    double percentile = (pixels > 0 ? (100.0 * counts / pixels) : 0.0);
    m_labelPercentileValue->setText(value.sprintf("%4.1f", percentile));
}

void HistogramPropsPlugin::applyChanges()
{
}

#include "histogrampropsplugin.moc"
