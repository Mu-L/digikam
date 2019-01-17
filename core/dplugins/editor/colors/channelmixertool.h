/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2005-02-26
 * Description : image channels mixer.
 *
 * Copyright (C) 2005-2019 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#ifndef DIGIKAM_EDITOR_CHANNEL_MIXER_TOOL_H
#define DIGIKAM_EDITOR_CHANNEL_MIXER_TOOL_H

// Local includes

#include "editortool.h"

namespace Digikam
{

class ChannelMixerTool : public EditorToolThreaded
{
    Q_OBJECT

public:

    explicit ChannelMixerTool(QObject* const parent);
    ~ChannelMixerTool();

private:

    void readSettings();
    void writeSettings();
    void preparePreview();
    void prepareFinal();
    void setPreviewImage();
    void setFinalImage();

    void updateSettingsWidgets();

private Q_SLOTS:

    void slotResetSettings();
    void slotSaveAsSettings();
    void slotLoadSettings();
    void slotOutChannelChanged();

private:

    class Private;
    Private* const d;
};

} // namespace Digikam

#endif // DIGIKAM_EDITOR_CHANNEL_MIXER_TOOL_H
