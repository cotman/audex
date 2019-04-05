/* AUDEX CDDA EXTRACTOR
 * Copyright (C) 2016 (audex@maniatek.com)
 * <http://userbase.kde.org/Audex>
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

#ifndef REPLAYGAINASSISTANT_H
#define REPLAYGAINASSISTANT_H

#include <QString>
#include <QStringList>

#include <KProcess>

#include "utils/encoderassistant.h"
#include "utils/patternparser.h"


/******************/
/* default values */
/******************/
#define REPLAYGAIN_MP3_BIN		"replaygain"
#define REPLAYGAIN_MP3_VERSION_PARA	"--version"

#define REPLAYGAIN_OGG_BIN		"vorbisgain"
#define REPLAYGAIN_OGG_VERSION_PARA	"--version"

#define REPLAYGAIN_FLAC_BIN		"metaflac"
#define REPLAYGAIN_FLAC_VERSION_PARA	"--version"

#define REPLAYGAIN_AAC_BIN		"replaygain"
#define REPLAYGAIN_AAC_VERSION_PARA	"--version"

// Assume python-rgain is fine here - it can actually do all the above formats, and more
#define REPLAYGAIN_CUSTOM_BIN		"replaygain"
#define REPLAYGAIN_CUSTOM_VERSION_PARA	"--version"


namespace ReplayGainAssistant {

  bool available(const EncoderAssistant::Encoder encoder);
  const QString pattern(const EncoderAssistant::Encoder encoder);

};

#endif
