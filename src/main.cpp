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

#include <QApplication>
#include <QCommandLineParser>

#include <KAboutData>
#include <KDBusService>
#include <Kdelibs4ConfigMigrator>
#include <KLocalizedString>


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    
    Kdelibs4ConfigMigrator migrate(QStringLiteral("kpeg"));
    migrate.setConfigFiles(QStringList() << QStringLiteral("kpegrc"));
    migrate.setUiFiles(QStringList() << QStringLiteral("kpegui.rc"));
    migrate.migrate();
  
    KAboutData aboutData(QLatin1String("kpeg"), i18n("KPeg"), QLatin1String("0.2"));
    aboutData.setShortDescription(i18n("Peg Solitaire game"));
    aboutData.setLicense(KAboutLicense::GPL);
    aboutData.setCopyrightStatement(i18n("(c) 2009, Graeme Gott\n"
                                    "(c) 2010, Ronny Yabar Aizcorbe\n"));
    aboutData.addAuthor(i18n("Ronny Yabar Aizcorbe"),
            i18n("Developer and current maintainer"),
            QLatin1String("ronnycontacto@gmail.com"));
    aboutData.addAuthor(i18n("Graeme Gott"), 
			i18n("Original author"), 
			QLatin1String("graeme@gottcode.org"));

    QCommandLineParser parser;
    KAboutData::setApplicationData(aboutData);
    parser.addVersionOption();
    parser.addHelpOption();
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("kpeg")));

    KpegMainWindow* window = new KpegMainWindow;
    window->show();

    return app.exec();
}
