/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/plugin_game5_dialog.cpp
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QMessageBox>
#include <QDir>
#include <QCompleter>
#include <QStringListModel>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_window.h"
#include "app/rdo_studio/src/model/model_tab_ctrl.h"
#include "app/rdo_studio/plugins/game5/src/plugin_game5_dialog.h"
#include "app/rdo_studio/plugins/game5/src/plugin_game5_tiles_order_dialog.h"
#include "app/rdo_studio/plugins/game5/src/multi_select_completer.h"
// --------------------------------------------------------------------------------

namespace
{
	enum moveDirection
	{
		MOVE_UP = 0,
		MOVE_DOWN,
		MOVE_RIGHT,
		MOVE_LEFT
	};
	const std::size_t HOLE_NUMBER = 0;
} // end anonymous namespace

PluginGame5GenerateSituationDialog::PluginGame5GenerateSituationDialog(QWidget* pParent)
	: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
	setupUi(this);
	gameBoard->init();

	MultiSelectCompleter* completer = new MultiSelectCompleter(QStringList(), this);
	lineEditCustom->setCompleter(completer);
	connect(lineEditCustom, &QLineEdit::selectionChanged, completer, &MultiSelectCompleter::onLineEditTextChanged);

	adjustSize();
	hiddenWidget->setFixedWidth(hiddenWidget->width());
	hiddenWidget->hide();

	buttonHide->setCheckable(true);
	buttonHide->setDefault(false);

	adjustSize();
	setFixedSize(width(), height());
	setSizeGripEnabled(false);

	rdo::gui::model::Model* pModel = getCurrentModel();
	clearAllTabs();
	pModel->getTab()->getItemEdit(rdo::model::FUN)->clearAll();
	pModel->getTab()->getItemEdit(rdo::model::FUN)->appendText(QString::fromStdString(modelFUN()));

	connect(buttonHide        , &QPushButton::toggled, this, &PluginGame5GenerateSituationDialog::onClickHide         );
	connect(buttonSetLineup   , &QPushButton::clicked, this, &PluginGame5GenerateSituationDialog::callTilesOrderDialog);
	connect(buttonRandomLineup, &QPushButton::clicked, this, &PluginGame5GenerateSituationDialog::emitSolvabilityCheck);
	connect(buttonOk          , &QPushButton::clicked, this, &PluginGame5GenerateSituationDialog::onClickOk           );

	connect(this     , &PluginGame5GenerateSituationDialog::buttonRandomClicked,
	        gameBoard, &Board::buildRandomOrder);
	connect(buttonRightLineup, &QPushButton::clicked,
	        gameBoard        , &Board::buildCorrectOrder);

	if (pParent)
	{
		move(pParent->frameGeometry().center() - frameGeometry().center());
	}
}

PluginGame5GenerateSituationDialog::~PluginGame5GenerateSituationDialog()
{}

void PluginGame5GenerateSituationDialog::onClickHide(bool state)
{
	hiddenWidget->setVisible(state);
	setFixedWidth(QWIDGETSIZE_MAX);
	adjustSize();
	setFixedWidth(width());
}

void PluginGame5GenerateSituationDialog::onClickOk()
{
	rdo::gui::model::Model* pModel = getCurrentModel();
	updateTabs();
	pModel->runModel();
	done(Accepted);
}

void PluginGame5GenerateSituationDialog::emitSolvabilityCheck()
{
	emit buttonRandomClicked(solvabilityCheck->isChecked());
}

std::string PluginGame5GenerateSituationDialog::evaluateBy() const
{
	if (radioButton0->isChecked())
	{
		return "0";
	}
	if (radioButtonQuantity->isChecked())
	{
		return "Кол_во_фишек_не_на_месте()";
	}
	if (radioButtonDistance->isChecked())
	{
		return "Расстояния_фишек_до_мест()";
	}
	else //if (radioButtonCustom->isChecked())
	{
		return lineEditCustom->text().toStdString();
	}
}

std::string PluginGame5GenerateSituationDialog::activityValue(int direction) const
{
	QString costValue;
	QString calcSwitcher;
	switch (direction)
	{
		case MOVE_DOWN:
			costValue    = moveDownCost->getLineEdit().text();
			calcSwitcher = moveDownCalcSwitcher->currentText();
			break;
		case MOVE_LEFT:
			costValue    = moveLeftCost->getLineEdit().text();
			calcSwitcher = moveLeftCalcSwitcher->currentText();
			break;
		case MOVE_RIGHT:
			costValue    = moveRightCost->getLineEdit().text();
			calcSwitcher = moveRightCalcSwitcher->currentText();
			break;
		case MOVE_UP:
			costValue    = moveUpCost->getLineEdit().text();
			calcSwitcher = moveUpCalcSwitcher->currentText();
			break;
		default:
			break;
	}
	const QString string = calcSwitcher + " " + costValue ;
	return string.toStdString();
}

std::string PluginGame5GenerateSituationDialog::modelRTP() const
{
	std::stringstream RTPtabTextStream; 
	RTPtabTextStream
		<< "$Resource_type Фишка : permanent" << std::endl
		<< "$Parameters" << std::endl
		<< "	Номер          : integer[1.." << gameBoard->getQuantityOfTiles()     << "]" << std::endl
		<< "	Местоположение : integer[1.." << gameBoard->getQuantityOfTiles() + 1 << "]" << std::endl
		<< "$End" << std::endl
		<< std::endl
		<< "$Resource_type Дырка_t : permanent" << std::endl
		<< "$Parameters" << std::endl
		<< "\t	Место: integer[1.." << gameBoard->getQuantityOfTiles() + 1 << "]" << std::endl
		<< "$End" << std::endl;
	return RTPtabTextStream.str();
}

std::string PluginGame5GenerateSituationDialog::modelRSS() const
{
	std::stringstream RSStabTextStream; 
	RSStabTextStream << "$Resources" << std::endl;
	for (int i = 1; i < gameBoard->getQuantityOfTiles() + 1; i++)
	{
		RSStabTextStream << "\tФишка" << i <<" = Фишка(" << i << ", " << gameBoard->getTilePosition(i) << ");" << std::endl;
	}
	RSStabTextStream << "\tДырка = Дырка_t(" << gameBoard->getTilePosition(HOLE_NUMBER) << ");" << std::endl;
	RSStabTextStream << "$End" << std::endl;
	return RSStabTextStream.str();
}

std::string PluginGame5GenerateSituationDialog::modelPAT() const
{
	std::stringstream PATtabTextStream; 
	PATtabTextStream
		<< "$Pattern Перемещение_фишки : rule" << std::endl
		<< "$Parameters" << std::endl
		<< "	Куда_перемещать: such_as Место_дырки" << std::endl
		<< "	На_сколько_перемещать: integer" << std::endl
		<< "$Relevant_resources" << std::endl
		<< "	_Фишка: Фишка   Keep" << std::endl
		<< "	_Дырка: Дырка_t Keep" << std::endl
		<< "$Body" << std::endl
		<< "	_Фишка:" << std::endl
		<< "		Choice from Где_дырка(_Фишка.Местоположение) == Куда_перемещать" << std::endl
		<< "		first" << std::endl
		<< "			Convert_rule Местоположение = _Фишка.Местоположение + На_сколько_перемещать;" << std::endl
		<< "	_Дырка:" << std::endl
		<< "		Choice NoCheck" << std::endl
		<< "		first" << std::endl
		<< "			Convert_rule Место = _Дырка.Место - На_сколько_перемещать;" << std::endl
		<< "$End" << std::endl;
	return PATtabTextStream.str();
}

std::string PluginGame5GenerateSituationDialog::modelDPT() const
{
	std::stringstream DPTtabTextStream; 
	DPTtabTextStream
		<< "$Decision_point Расстановка_фишек : search trace_all" << std::endl
		<< "$Condition Exist(Фишка: Фишка.Номер <> Фишка.Местоположение)" << std::endl
		<< "$Term_condition" << std::endl
		<< "	For_All(Фишка: Фишка.Номер == Фишка.Местоположение)" << std::endl
		<< "$Evaluate_by " << evaluateBy() << std::endl
		<< "$Compare_tops = " << (checkBoxCopareTop->isChecked() ? "YES" : "NO") << std::endl
		<< "$Activities" << std::endl
		<< "	Перемещение_вправо: Перемещение_фишки справа  1 value " << activityValue(MOVE_LEFT) << std::endl
		<< "	Перемещение_влево : Перемещение_фишки слева  -1 value " << activityValue(MOVE_RIGHT) << std::endl
		<< "	Перемещение_вверх : Перемещение_фишки сверху -" << gameBoard->m_tilesCountX << " value " << activityValue(MOVE_DOWN) << std::endl
		<< "	Перемещение_вниз  : Перемещение_фишки снизу   " << gameBoard->m_tilesCountX << " value " << activityValue(MOVE_UP) << std::endl
		<< "$End";
	return DPTtabTextStream.str();
}

std::string PluginGame5GenerateSituationDialog::modelFUN() const
{
	std::stringstream FUNtabTextStream; 
	FUNtabTextStream
		<< "$Constant" << std::endl
		<< "	Место_дырки: (справа, слева, сверху, снизу, дырки_рядом_нет) = дырки_рядом_нет" << std::endl
		<< "	Длина_поля : integer = " << gameBoard->m_tilesCountX << std::endl
		<< "$End" << std::endl
		<< std::endl
		<< "$Function Ряд: integer" << std::endl
		<< "$Type = algorithmic" << std::endl
		<< "$Parameters" << std::endl
		<< "	Местоположение: integer" << std::endl
		<< "$Body" << std::endl
		<< "	return (Местоположение - 1)/Длина_поля + 1;" << std::endl
		<< "$End" << std::endl
		<< std::endl
		<< "$Function Остаток_от_деления : integer" << std::endl
		<< "$Type = algorithmic" << std::endl
		<< "$Parameters" << std::endl
		<< "	Делимое   : integer" << std::endl
		<< "	Делитель  : integer" << std::endl
		<< "$Body" << std::endl
		<< "	integer Целая_часть  = Делимое/Делитель;" << std::endl
		<< "	integer Макс_делимое = Делитель * int(Целая_часть);" << std::endl
		<< "	return Делимое -  Макс_делимое;" << std::endl
		<< "$End" << std::endl
		<< std::endl
		<< "$Function Столбец: integer" << std::endl
		<< "$Type = algorithmic" << std::endl
		<< "$Parameters" << std::endl
		<< "	Местоположение: integer" << std::endl
		<< "$Body" << std::endl
		<< "	return Остаток_от_деления(Местоположение - 1,Длина_поля) + 1;" << std::endl
		<< "$End" << std::endl
		<< std::endl
		<< "$Function Где_дырка : such_as Место_дырки" << std::endl
		<< "$Type = algorithmic" << std::endl
		<< "$Parameters" << std::endl
		<< "	_Место: such_as Фишка.Местоположение" << std::endl
		<< "$Body" << std::endl
		<< "	if (Столбец(_Место) == Столбец(Дырка.Место) and Ряд(_Место) == Ряд(Дырка.Место)+ 1) return сверху;" << std::endl
		<< "	if (Столбец(_Место) == Столбец(Дырка.Место) and Ряд(_Место) == Ряд(Дырка.Место)- 1) return снизу;" << std::endl
		<< "	if (Ряд(_Место) == Ряд(Дырка.Место) and Столбец(_Место) == Столбец(Дырка.Место)- 1) return справа;" << std::endl
		<< "	if (Ряд(_Место) == Ряд(Дырка.Место) and Столбец(_Место) == Столбец(Дырка.Место)+ 1) return слева;" << std::endl
		<< "	return дырки_рядом_нет;" << std::endl
		<< "$End" << std::endl
		<< std::endl
		<< "$Function Фишка_на_месте : integer" << std::endl
		<< "$Type = algorithmic" << std::endl
		<< "$Parameters" << std::endl
		<< "	_Номер: such_as Фишка.Номер" << std::endl
		<< "	_Место: such_as Фишка.Местоположение" << std::endl
		<< "$Body" << std::endl
		<< "	if (_Номер == _Место) return 1;" << std::endl
		<< "	else                  return 0;" << std::endl
		<< "$End" << std::endl
		<< std::endl
		<< "$Function Кол_во_фишек_не_на_месте : integer" << std::endl
		<< "$Type = algorithmic" << std::endl
		<< "$Parameters" << std::endl
		<< "$Body" << std::endl
		<< "	return " << gameBoard->getQuantityOfTiles() << " - (Фишка_на_месте(Фишка1.Номер, Фишка1.Местоположение)+" << std::endl;
	for (int i = 2; i < gameBoard->getQuantityOfTiles(); i++)
	{
		FUNtabTextStream
			<< "	            Фишка_на_месте(Фишка" << i <<".Номер, Фишка" << i <<".Местоположение)+" << std::endl;
	}
	FUNtabTextStream
		<< "	            Фишка_на_месте(Фишка" << gameBoard->getQuantityOfTiles() <<".Номер, Фишка" << gameBoard->getQuantityOfTiles() <<".Местоположение));" << std::endl
		<< "$End" << std::endl
		<< std::endl
		<< "$Function Расстояние_фишки_до_места : integer" << std::endl
		<< "$Type = algorithmic" << std::endl
		<< "$Parameters" << std::endl
		<< "	Откуда: integer" << std::endl
		<< "	Куда  : integer" << std::endl
		<< "$Body" << std::endl
		<< "	return Abs(Ряд(Откуда)-Ряд(Куда)) + Abs(Столбец(Откуда)-Столбец(Куда));" << std::endl
		<< "$End" << std::endl
		<< std::endl
		<< "$Function Расстояния_фишек_до_мест : integer" << std::endl
		<< "$Type = algorithmic" << std::endl
		<< "$Parameters" << std::endl
		<< "$Body" << std::endl
		<< "	return Расстояние_фишки_до_места(Фишка1.Номер, Фишка1.Местоположение)+" << std::endl;
	for (int i = 2; i < gameBoard->getQuantityOfTiles(); i++)
	{
		FUNtabTextStream
			<< "	       Расстояние_фишки_до_места(Фишка" << i << ".Номер, Фишка" << i << ".Местоположение)+" << std::endl;
	}
	FUNtabTextStream
		<< "	       Расстояние_фишки_до_места(Фишка" << gameBoard->getQuantityOfTiles() << ".Номер, Фишка" << gameBoard->getQuantityOfTiles() << ".Местоположение);" << std::endl
		<< "$End";
	return FUNtabTextStream.str();
}

void PluginGame5GenerateSituationDialog::clearAllTabs() const
{
	rdo::gui::model::Model* pModel = getCurrentModel();
	for (int i = 0; i < pModel->getTab()->tabBar()->count(); i++)
	{
		if (i != rdo::model::FUN)
		{
			pModel->getTab()->getItemEdit(i)->clearAll();
		}
	}
}

void PluginGame5GenerateSituationDialog::callTilesOrderDialog()
{
	TilesOrderDialog dlg(this, gameBoard->getBoardState());
	connect(&dlg, &TilesOrderDialog::tilesOrderCommited, gameBoard, &Board::setTilesPositon);
	dlg.exec();
}

void PluginGame5GenerateSituationDialog::updateTabs() const
{
	rdo::gui::model::Model* pModel = getCurrentModel();
	if (pModel->getTab())
	{
		clearAllTabs();

		pModel->getTab()->getItemEdit(rdo::model::RTP)->appendText(QString::fromStdString(modelRTP()));
		pModel->getTab()->getItemEdit(rdo::model::RSS)->appendText(QString::fromStdString(modelRSS()));
		pModel->getTab()->getItemEdit(rdo::model::PAT)->appendText(QString::fromStdString(modelPAT()));
		pModel->getTab()->getItemEdit(rdo::model::DPT)->appendText(QString::fromStdString(modelDPT()));
	}
}

void PluginGame5GenerateSituationDialog::onPluginAction()
{
	QStringList funList = parseModelFUN();
	QStringListModel* stringModel = (QStringListModel*)lineEditCustom->completer()->model();
	stringModel->setStringList(funList);
	exec();
}

QStringList PluginGame5GenerateSituationDialog::parseModelFUN() const
{
	rdo::gui::model::Model* pModel = getCurrentModel();
	std::stringstream txtStream;
	pModel->getTab()->getItemEdit(rdo::model::FUN)->save(txtStream);
	QString tabStr = QString::fromStdString(txtStream.str());
	QRegExp regExp("(\\$Function)(\\s*)([A-Za-z0-9_А-Яа-я\\$]*)(\\s*):");

	QStringList list;
	int pos = 0;
	while ((pos = regExp.indexIn(tabStr, pos))!= -1)
	{
		list << regExp.cap(3);
		pos += regExp.matchedLength();
	}
	return list;
}

rdo::gui::model::Model* PluginGame5GenerateSituationDialog::getCurrentModel() const
{
	MainWindow* pMainWindow = (MainWindow*)(parent());
	return pMainWindow->getModel();
}

std::vector<unsigned int> PluginGame5GenerateSituationDialog::getBoardState() const
{
	return gameBoard->getBoardState();
}
