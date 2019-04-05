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

#include "replaygainassistant.h"


bool ReplayGainAssistant::available(const EncoderAssistant::Encoder encoder) {

  switch (encoder) {

    case EncoderAssistant::LAME : return (KProcess::execute(REPLAYGAIN_MP3_BIN, QStringList() << REPLAYGAIN_MP3_VERSION_PARA)==0);
    case EncoderAssistant::OGGENC : return (KProcess::execute(REPLAYGAIN_OGG_BIN, QStringList() << REPLAYGAIN_OGG_VERSION_PARA)==0);
    case EncoderAssistant::FLAC : return (KProcess::execute(REPLAYGAIN_FLAC_BIN, QStringList() << REPLAYGAIN_FLAC_VERSION_PARA)==0);
    case EncoderAssistant::FAAC : return (KProcess::execute(REPLAYGAIN_AAC_BIN, QStringList() << REPLAYGAIN_AAC_VERSION_PARA)==0);
    case EncoderAssistant::WAVE : return false;
    case EncoderAssistant::CUSTOM : return (KProcess::execute(REPLAYGAIN_CUSTOM_BIN, QStringList() << REPLAYGAIN_CUSTOM_VERSION_PARA)==0);
    default : return false;

  }

}

const QString ReplayGainAssistant::pattern(const EncoderAssistant::Encoder encoder) {

  QString cmd = QString();

  switch (encoder) {

    case EncoderAssistant::LAME : {
      cmd += REPLAYGAIN_MP3_BIN;
      cmd += " %1/*.$" VAR_SUFFIX "";
      return cmd;
    }

    case EncoderAssistant::OGGENC : {
      cmd += REPLAYGAIN_OGG_BIN;
      cmd += " -a -f -r %1/*.$" VAR_SUFFIX "";
      return cmd;
    }

    case EncoderAssistant::FLAC : {
      cmd += REPLAYGAIN_FLAC_BIN;
      cmd += " --add-replay-gain %1/*.$" VAR_SUFFIX "";
      return cmd;
    }

    case EncoderAssistant::FAAC : {
      cmd += REPLAYGAIN_AAC_BIN;
      cmd += " %1/*.$" VAR_SUFFIX "";
      return cmd;
    }

    case EncoderAssistant::CUSTOM : {
      cmd += REPLAYGAIN_CUSTOM_BIN;
      cmd += " %1/*.$" VAR_SUFFIX "";
      return cmd;
    }

    default : ;

  }

  return cmd;

}
