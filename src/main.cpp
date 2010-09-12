/*
  Copyright 2009  Graeme Gott <graeme@gottcode.org>
  Copyright 2010  Ronny Yabar Aizcorbe <ronnycontacto@gmail.com>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of
  the License or (at your option) version 3 or any later version
  accepted by the membership of KDE e.V. (or its successor approved
  by the membership of KDE e.V.), which shall act as a proxy
  defined in Section 14 of version 3 of the license.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "window.h"

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>

static KLocalizedString description = ki18n("Peg Solitaire game");
static KLocalizedString notice = ki18n("(c) 2009, Graeme Gott\n"
"(c) 2010, Ronny Yabar Aizcorbe\n");

static const char version[] = "0.2";

int main(int argc, char* argv[])
{
  
    KAboutData aboutData( "kpeg", 0, ki18n("KPeg"),
    version, description, KAboutData::License_GPL, notice);
    aboutData.addAuthor(ki18n("Graeme Gott"), ki18n("Original author"), "graeme@gottcode.org");
    aboutData.addAuthor(ki18n("Ronny Yabar Aizcorbe"), ki18n("Developer and current maintainer"), "ronnycontacto@gmail.com");			 
    KCmdLineArgs::init(argc, argv, &aboutData);

    KApplication app;
    KGlobal::locale()->insertCatalog("libkdegames");

    KpegMainWindow* window = new KpegMainWindow;
    window->show();

    return app.exec();
}
