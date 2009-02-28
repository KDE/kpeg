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

#include "window.h"

#include "board.h"
#include "settings.h"

#include <KApplication>
#include <KAction>
#include <KActionCollection>
#include <KComboBox>
#include <KConfigDialog>
#include <KConfigGroup>
#include <KDialog>
#include <KGameThemeSelector>
#include <KGlobal>
#include <KLocale>
#include <KMessageBox>
#include <KSharedConfig>
#include <KStandardGameAction>
#include <KStatusBar>
#include <KUndoStack>

#include <QFormLayout>
#include <QSpinBox>
#include <QVBoxLayout>

#include <ctime>

/*****************************************************************************/

Window::Window(QWidget* parent)
: KXmlGuiWindow(parent),
  m_seed(0),
  m_difficulty(5),
  m_algorithm(1) {
	// Create moves history
	m_moves = new KUndoStack(this);
	KAction* action = KStandardGameAction::undo(m_moves, SLOT(undo()), actionCollection());
	action->setEnabled(false);
	connect(m_moves, SIGNAL(canUndoChanged(bool)), action, SLOT(setEnabled(bool)));
	action = KStandardGameAction::redo(m_moves, SLOT(redo()), actionCollection());
	action->setEnabled(false);
	connect(m_moves, SIGNAL(canRedoChanged(bool)), action, SLOT(setEnabled(bool)));

	// Create board
	m_board = new Board(m_moves, this);
	setCentralWidget(m_board);

	// Create actions
	KStandardGameAction::gameNew(this, SLOT(newGame()), actionCollection());
	KStandardGameAction::restart(this, SLOT(restartGame()), actionCollection());
	KStandardGameAction::quit(kapp, SLOT(quit()), actionCollection());
	KStandardAction::preferences(this, SLOT(configureSettings()), actionCollection());

	// Setup statusbar
	statusBar()->insertPermanentItem(QString(), 1);

	QFrame* divider = new QFrame(statusBar());
	divider->setFrameStyle(QFrame::VLine);
	statusBar()->addPermanentWidget(divider);

	statusBar()->insertPermanentItem(QString(), 2);

	divider = new QFrame(statusBar());
	divider->setFrameStyle(QFrame::VLine);
	statusBar()->addPermanentWidget(divider);

	statusBar()->insertPermanentItem(QString(), 3);

	setupGUI();

	loadGame();
}

/*****************************************************************************/

Window::~Window() {
	// Save current game
	if (m_moves->count()) {
		QStringList moves;
		int count = m_moves->index();
		for (int i = 0; i < count; ++i) {
			moves += m_moves->text(i);
		}
		KConfigGroup savegame = KGlobal::config()->group("Game");
		savegame.writeEntry("Moves", moves);
	}
}

/*****************************************************************************/

void Window::newGame() {
	KDialog* dialog = new KDialog(this);
	dialog->setCaption(i18n("New"));
	dialog->setButtons(KDialog::Ok | KDialog::Cancel);

	// Create contents of dialog
	QWidget* contents = new QWidget(dialog);
	dialog->setMainWidget(contents);

	QSpinBox* seed_box = new QSpinBox(contents);
	seed_box->setRange(0, RAND_MAX);
	seed_box->setValue(0);
	seed_box->setSpecialValueText(i18n("Random"));

	QSpinBox* difficulty_box = new QSpinBox(contents);
	difficulty_box->setRange(1, 100);
	difficulty_box->setValue(m_difficulty);

	KComboBox* algorithms_box = new KComboBox(contents);
	algorithms_box->addItem(i18n("Original"), 1);
	algorithms_box->addItem(i18n("Branch"), 2);
	algorithms_box->addItem(i18n("Line"), 3);
	algorithms_box->setCurrentIndex(algorithms_box->findData(m_algorithm));

	// Layout dialog
	QFormLayout* box_layout = new QFormLayout(contents);
	box_layout->addRow(i18n("Game Number:"), seed_box);
	box_layout->addRow(i18n("Difficulty:"), difficulty_box);
	box_layout->addRow(i18n("Algorithm:"), algorithms_box);

	if (dialog->exec() == QDialog::Accepted) {
		startGame(seed_box->value(), difficulty_box->value(), algorithms_box->itemData(algorithms_box->currentIndex()).toInt());
	}

	delete dialog;
}

/*****************************************************************************/

void Window::restartGame() {
	if (m_board->isFinished() || KMessageBox::questionYesNo(0, i18n("Do you want to restart?")) == KMessageBox::Yes) {
		m_board->generate(m_seed, m_difficulty, m_algorithm);
	}
}

/*****************************************************************************/

void Window::configureSettings() {
	if (KConfigDialog::showDialog("settings")) {
		return;
	}
	KConfigDialog* dialog = new KConfigDialog(this, "settings", PegeSettings::self());
	dialog->addPage(new KGameThemeSelector(dialog, PegeSettings::self(), KGameThemeSelector::NewStuffDisableDownload), i18n("Theme"), "games-config-theme");
	connect(dialog, SIGNAL(settingsChanged(const QString&)), this, SLOT(loadSettings()));
	dialog->show();
}

/*****************************************************************************/

void Window::loadSettings() {
	m_board->setTheme(PegeSettings::theme());
}

/*****************************************************************************/

void Window::loadGame() {
	// Load board
	KConfigGroup savegame = KGlobal::config()->group("Game");
	int seed = savegame.readEntry("Seed", 0);
	int difficulty = savegame.readEntry("Difficulty", 5);
	int algorithm = savegame.readEntry("Algorithm", 1);
	QStringList moves = savegame.readEntry("Moves", QStringList());
	startGame(seed, difficulty, algorithm);

	// Load moves
	QRegExp parse("(-?\\d+)x(-?\\d+):(-?\\d+)x(-?\\d+)");
	foreach (const QString& move, moves) {
		if (!parse.exactMatch(move)) {
			continue;
		}

		QPoint old_hole(parse.cap(1).toInt(), parse.cap(2).toInt());
		QPoint new_hole(parse.cap(3).toInt(), parse.cap(4).toInt());
		if (m_board->isPeg(old_hole) && m_board->isHole(new_hole)) {
			m_board->move(old_hole, new_hole);
		} else {
			qWarning("Invalid move: %dx%d to %dx%d", old_hole.x(), old_hole.y(), new_hole.x(), new_hole.y());
		}
	}
}

/*****************************************************************************/

void Window::startGame(int seed, int difficulty, int algorithm) {
	m_algorithm = algorithm;
	QString text;
	switch (m_algorithm) {
	case 1:
		text = i18n("Original");
		break;
	case 2:
		text = i18n("Branch");
		break;
	case 3:
		text = i18n("Line");
		break;
	default:
		break;
	}
	statusBar()->changeItem(i18n("Algorithm: %1", text), 1);

	m_difficulty = difficulty;
	statusBar()->changeItem(i18n("Difficulty: %1", m_difficulty), 2);

	if (seed == 0) {
		srand(time(0));
		seed = rand();
	}
	m_seed = seed;
	statusBar()->changeItem(i18n("Game Number: %1", m_seed), 3);

	m_board->generate(m_seed, m_difficulty, m_algorithm);

	KConfigGroup savegame = KGlobal::config()->group("Game");
	savegame.writeEntry("Seed", m_seed);
	savegame.writeEntry("Difficulty", m_difficulty);
	savegame.writeEntry("Algorithm", m_algorithm);
	savegame.writeEntry("Moves", QStringList());
}

/*****************************************************************************/
