/*
  Copyright 2009  Graeme Gott <graeme@gottcode.org>

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

/**
 * @mainpage Peg-E
 *
 * Peg-E is a peg elimination game.
 *
 * @author Graeme Gott
 */

#include "window.h"

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>

int main(int argc, char* argv[])
{
    KAboutData about_data("pege", 0, ki18n("Peg-E"), "1.0.90",
                          ki18n("Peg elimination game"), KAboutData::License_GPL,
                          ki18n("Copyright &copy; 2009 Graeme Gott"), KLocalizedString(), 0,
                          "graeme@gottcode.org");
    about_data.addAuthor(ki18n("Graeme Gott"), KLocalizedString(),
                         "graeme@gottcode.org", "http://gottcode.org/");
    KCmdLineArgs::init(argc, argv, &about_data);

    KApplication app;
    KGlobal::locale()->insertCatalog("libkdegames");

    PegeMainWindow* window = new PegeMainWindow;
    window->show();

    return app.exec();
}
