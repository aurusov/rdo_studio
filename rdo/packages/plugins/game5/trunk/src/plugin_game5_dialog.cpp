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
#include <QMessageBox>
#include <QDir>
#include <QCompleter>
#include <QStringListModel>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_window.h"
#include "app/rdo_studio/src/model/model_tab_ctrl.h"
#include "app/rdo_studio/plugins/game5/src/plugin_game5_dialog.h"
#include "app/rdo_studio/plugins/game5/src/plugin_game5_tiles_order_dialog.h"
#include "app/rdo_studio/plugins/game5/src/cost_setup_table.h"
#include "app/rdo_studio/plugins/game5/src/multi_select_completer.h"
// --------------------------------------------------------------------------------

PluginGame5GenerateSituationDialog::PluginGame5GenerateSituationDialog(QWidget* pParent)
	: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
	setupUi(this);
	gameBoard->init();

	hiddenWidget->setFixedWidth(280);
	hiddenWidget->hide();

	buttonHide->setCheckable(true);
	buttonHide->setDefault(false);

	MultiSelectCompleter* completer = new MultiSelectCompleter(QStringList(), this);
	lineEditCustom->setCompleter(completer);

	setFixedHeight(std::max(375, gameBoard->m_boardSizeY + 160));
	adjustSize();
	setFixedWidth(width());
	setSizeGripEnabled(false);

	rdo::gui::model::Model* pModel = getCurrentModel();
	clearAllTabs();
	pModel->getTab()->getItemEdit(rdoModelObjects::FUN)->clearAll();
	pModel->getTab()->getItemEdit(rdoModelObjects::FUN)->appendText(QString::fromStdString(FUNtabText()));

	connect(buttonHide        , &QPushButton::toggled, this, &PluginGame5GenerateSituationDialog::onClickHide         );
	connect(buttonSetLineup   , &QPushButton::clicked, this, &PluginGame5GenerateSituationDialog::callDialog          );
	connect(buttonRandomLineup, &QPushButton::clicked, this, &PluginGame5GenerateSituationDialog::emitSolvabilityCheck);
	connect(buttonOk          , &QPushButton::clicked, this, &PluginGame5GenerateSituationDialog::onClickOk           );

	connect(tableCostValue, &CostSetupTable::itemCheckStateChanged,
	        this          , &PluginGame5GenerateSituationDialog::onItemCheckStateChanged
	);
	connect(this     , &PluginGame5GenerateSituationDialog::buttonRandomClicked,
	        gameBoard, &Board::buildRandomLineup
	);
	connect(buttonRightLineup, &QPushButton::clicked,
	        gameBoard        , &Board::buildRightLineup
	);

	if (pParent)
	{
		move(pParent->frameGeometry().center() - frameGeometry().center());
	}
}

PluginGame5GenerateSituationDialog::~PluginGame5GenerateSituationDialog()
{
}

void PluginGame5GenerateSituationDialog::onClickHide(bool state)
{
	hiddenWidget->setVisible(state);
	buttonHide->setText(buttonHide->text() == ">>" ? "<<" : ">>");
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

void PluginGame5GenerateSituationDialog::onItemCheckStateChanged(QTableWidgetItem* item)
{
	QTableWidget* parentTable = item->tableWidget();
	if (item->checkState() == Qt::Unchecked)
	{
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		item->setText("1");
	}
	else
	{
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		parentTable->edit(parentTable->model()->index(item->row(), item->column()));
	}
}

std::string PluginGame5GenerateSituationDialog::evaluateBy()
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

std::string PluginGame5GenerateSituationDialog::activityValue(int tableRow)
{
	QString string = tableCostValue->item(tableRow, 1)->text() + " " + tableCostValue->item(tableRow, 2)->text();
	return string.toStdString();
}

std::string PluginGame5GenerateSituationDialog::RTPtabText()
{
	std::stringstream RTPtabTextStream; 
	RTPtabTextStream
	<<	"$Resource_type Фишка : permanent\n"
	<<	"$Parameters\n"
	<<	"	Номер          : integer[1.." << (gameBoard->m_tilesCountX * gameBoard->m_tilesCountY)- 1 << "]\n"
	<<	"	Местоположение : integer[1.." << (gameBoard->m_tilesCountX * gameBoard->m_tilesCountY)    << "]\n"
	<<	"$End\n"
	<<	"\n"
	<<	"$Resource_type Дырка_t : permanent\n"
	<<	"$Parameters\n"
	<<	"\t	Место: integer[1.." << (gameBoard->m_tilesCountX * gameBoard->m_tilesCountY) << "]\n"
	<<	"$End\n";
	return RTPtabTextStream.str();
}

std::string PluginGame5GenerateSituationDialog::RSStabText()
{
	std::stringstream RSStabTextStream; 
	RSStabTextStream <<	"$Resources\n";
	for (unsigned int i = 1; i < gameBoard->getTilesPos().size(); i++)
	{
		RSStabTextStream << "\tФишка Фишка" << i <<" = new Фишка(" << i << ", " << gameBoard->getTilesPos()[i] << ");\n";
	}
	RSStabTextStream << "\tДырка_t Дырка = new Дырка_t(" << gameBoard->getTilesPos()[0] << ");\n";
	RSStabTextStream <<	"$End\n";
	return RSStabTextStream.str();
}

std::string PluginGame5GenerateSituationDialog::PATtabText()
{
	std::stringstream PATtabTextStream; 
	PATtabTextStream
	<<	"$Pattern Перемещение_фишки : rule\n"
	<<	"$Parameters\n"
	<<	"	Куда_перемещать: such_as Место_дырки\n"
	<<	"	На_сколько_перемещать: integer\n"
	<<	"$Relevant_resources\n"
	<<	"	_Фишка: Фишка   Keep\n"
	<<	"	_Дырка: Дырка_t Keep\n"
	<<	"$Body\n"
	<<	"	_Фишка\n"
	<<	"		Choice from Где_дырка(_Фишка.Местоположение) == Куда_перемещать\n"
	<<	"		first\n"
	<<	"			Convert_rule Местоположение = _Фишка.Местоположение + На_сколько_перемещать;\n"
	<<	"	_Дырка\n"
	<<	"		Choice NoCheck\n"
	<<	"		first\n"
	<<	"			Convert_rule Место = _Дырка.Место - На_сколько_перемещать;\n"
	<<	"$End\n";
	return PATtabTextStream.str();
}

std::string PluginGame5GenerateSituationDialog::DPTtabText()
{
	std::stringstream DPTtabTextStream; 
	DPTtabTextStream
	<<	"$Decision_point Расстановка_фишек : search trace_all\n"
	<<	"$Condition Exist(Фишка: Фишка.Номер <> Фишка.Местоположение)\n"
	<<	"$Term_condition\n"
	<<	"	For_All(Фишка: Фишка.Номер == Фишка.Местоположение)\n"
	<<	"$Evaluate_by " << evaluateBy() << "\n"
	<<	"$Compare_tops = " << (checkBoxCopareTop->isChecked() ? "YES" : "NO") << "\n"
	<<	"$Activities\n"
	<<	"	Перемещение_вправо: Перемещение_фишки справа  1 value " << activityValue(0) << "\n"
	<<	"	Перемещение_влево : Перемещение_фишки слева  -1 value " << activityValue(1) << "\n"
	<<	"	Перемещение_вверх : Перемещение_фишки сверху -" << gameBoard->m_tilesCountX << " value " << activityValue(2) << "\n"
	<<	"	Перемещение_вниз  : Перемещение_фишки снизу   " << gameBoard->m_tilesCountX << " value " << activityValue(3) << "\n"
	<<	"$End";
	return DPTtabTextStream.str();
}

std::string PluginGame5GenerateSituationDialog::FUNtabText()
{
	std::stringstream FUNtabTextStream; 
	FUNtabTextStream
	<<	"$Constant\n"
	<<	"	Место_дырки: (справа, слева, сверху, снизу, дырки_рядом_нет) = дырки_рядом_нет\n"
	<<	"	Длина_поля : integer = " << gameBoard->m_tilesCountX << "\n"
	<<	"$End\n"
	<<	"\n"
	<<	"$Function Ряд: integer\n"
	<<	"$Type = algorithmic\n"
	<<	"$Parameters\n"
	<<	"	Местоположение: integer\n"
	<<	"$Body\n"
	<<	"	return (Местоположение - 1)/Длина_поля + 1;\n"
	<<	"$End\n"
	<<	"\n"
	<<	"$Function Остаток_от_деления : integer\n"
	<<	"$Type = algorithmic\n"
	<<	"$Parameters\n"
	<<	"	Делимое   : integer\n"
	<<	"	Делитель  : integer\n"
	<<	"$Body\n"
	<<	"	integer Целая_часть  = Делимое/Делитель;\n"
	<<	"	integer Макс_делимое = Делитель * int(Целая_часть);\n"
	<<	"	return Делимое -  Макс_делимое;\n"
	<<	"$End\n"
	<<	"\n"
	<<	"$Function Столбец: integer\n"
	<<	"$Type = algorithmic\n"
	<<	"$Parameters\n"
	<<	"	Местоположение: integer\n"
	<<	"$Body\n"
	<<	"	return Остаток_от_деления(Местоположение - 1,Длина_поля) + 1;\n"
	<<	"$End\n"
	<<	"\n"
	<<	"$Function Где_дырка : such_as Место_дырки\n"
	<<	"$Type = algorithmic\n"
	<<	"$Parameters\n"
	<<	"	_Место: such_as Фишка.Местоположение\n"
	<<	"$Body\n"
	<<	"	if (Столбец(_Место) == Столбец(Дырка.Место) and Ряд(_Место) == Ряд(Дырка.Место)+ 1) return сверху;\n"
	<<	"	if (Столбец(_Место) == Столбец(Дырка.Место) and Ряд(_Место) == Ряд(Дырка.Место)- 1) return снизу;\n"
	<<	"	if (Ряд(_Место) == Ряд(Дырка.Место) and Столбец(_Место) == Столбец(Дырка.Место)- 1) return справа;\n"
	<<	"	if (Ряд(_Место) == Ряд(Дырка.Место) and Столбец(_Место) == Столбец(Дырка.Место)+ 1) return слева;\n"
	<<	"	return дырки_рядом_нет;\n"
	<<	"$End\n"
	<<	"\n"
	<<	"$Function Фишка_на_месте : integer\n"
	<<	"$Type = algorithmic\n"
	<<	"$Parameters\n"
	<<	"	_Номер: such_as Фишка.Номер\n"
	<<	"	_Место: such_as Фишка.Местоположение\n"
	<<	"$Body\n"
	<<	"	if (_Номер == _Место) return 1;\n"
	<<	"	else                  return 0;\n"
	<<	"$End\n"
	<<	"\n"
	<<	"$Function Кол_во_фишек_не_на_месте : integer\n"
	<<	"$Type = algorithmic\n"
	<<	"$Parameters\n"
	<<	"$Body\n"
	<<	"	return " << gameBoard->getTilesPos().size() - 1 << " - (Фишка_на_месте(Фишка1.Номер, Фишка1.Местоположение)+\n";
	for (unsigned int i = 2; i < gameBoard->getTilesPos().size() - 1; i++)
	{
		FUNtabTextStream
	<<	"	            Фишка_на_месте(Фишка" << i <<".Номер, Фишка" << i <<".Местоположение)+\n";
	}
	FUNtabTextStream
	<<	"	            Фишка_на_месте(Фишка" << gameBoard->getTilesPos().size() - 1 <<".Номер, Фишка" << gameBoard->getTilesPos().size() - 1 <<".Местоположение));\n"
	<<	"$End\n"
	<<	"\n"
	<<	"$Function Расстояние_фишки_до_места : integer\n"
	<<	"$Type = algorithmic\n"
	<<	"$Parameters\n"
	<<	"	Откуда: integer\n"
	<<	"	Куда  : integer\n"
	<<	"$Body\n"
	<<	"	return Abs(Ряд(Откуда)-Ряд(Куда)) + Abs(Столбец(Откуда)-Столбец(Куда));\n"
	<<	"$End\n"
	<<	"\n"
	<<	"$Function Расстояния_фишек_до_мест : integer\n"
	<<	"$Type = algorithmic\n"
	<<	"$Parameters\n"
	<<	"$Body\n"
	<<	"	return Расстояние_фишки_до_места(Фишка1.Номер, Фишка1.Местоположение)+\n";
	for (unsigned int i = 2; i < gameBoard->getTilesPos().size() - 1; i++)
	{
		FUNtabTextStream
	<<	"	       Расстояние_фишки_до_места(Фишка" << i << ".Номер, Фишка" << i << ".Местоположение)+\n";
	}
	FUNtabTextStream
	<<	"	       Расстояние_фишки_до_места(Фишка" << gameBoard->getTilesPos().size() - 1 << ".Номер, Фишка" << gameBoard->getTilesPos().size() - 1 << ".Местоположение);\n"
	<<	"$End";
	return FUNtabTextStream.str();
}

void PluginGame5GenerateSituationDialog::clearAllTabs()
{
	rdo::gui::model::Model* pModel = getCurrentModel();
	for (int i = 0; i < pModel->getTab()->tabBar()->count(); i++)
	{
		if (i != rdoModelObjects::FUN)
		{
			pModel->getTab()->getItemEdit(i)->clearAll();
		}
	}
}

void PluginGame5GenerateSituationDialog::callDialog()
{
	TilesOrderDialog dlg(this, gameBoard->getBoardState());
	connect(&dlg, &TilesOrderDialog::tilesOrderCommited, gameBoard, &Board::setTilesPositon);
	dlg.exec();
}

void PluginGame5GenerateSituationDialog::updateTabs()
{
	rdo::gui::model::Model* pModel = getCurrentModel();
	if (pModel->getTab())
	{
		clearAllTabs();

		pModel->getTab()->getItemEdit(rdoModelObjects::RTP)->appendText(QString::fromStdString(RTPtabText()));
		pModel->getTab()->getItemEdit(rdoModelObjects::RSS)->appendText(QString::fromStdString(RSStabText()));
		pModel->getTab()->getItemEdit(rdoModelObjects::PAT)->appendText(QString::fromStdString(PATtabText()));
		pModel->getTab()->getItemEdit(rdoModelObjects::DPT)->appendText(QString::fromStdString(DPTtabText()));
	}
}

void PluginGame5GenerateSituationDialog::onPlgnAction()
{
	QStringList funList = parseFunTab();
	QStringListModel* stringModel = (QStringListModel*)lineEditCustom->completer()->model();
	stringModel->setStringList(funList);
	exec();
}

QStringList PluginGame5GenerateSituationDialog::parseFunTab()
{
	rdo::gui::model::Model* pModel = getCurrentModel();
	rdo::textstream txtStream;
	pModel->getTab()->getItemEdit(rdoModelObjects::FUN)->save(txtStream);
	QString tabStr = QString::fromStdString(txtStream.str());
	QRegExp regExp("(\\$Function)(\\s*)([A-Za-z0-9_А-Яа-я\\$]*)(\\s*):");

	QStringList list;
	int pos=0;
	while((pos = regExp.indexIn(tabStr, pos))!= -1)
	{
		list << regExp.cap(3);
		pos += regExp.matchedLength();
	}
	return list;
}

rdo::gui::model::Model* PluginGame5GenerateSituationDialog::getCurrentModel()
{
	MainWindow* pMainWindow = (MainWindow*)(parent());
	return pMainWindow->getModel();
}

QString PluginGame5GenerateSituationDialog::getBoardState()
{
	return gameBoard->getBoardState();
}
