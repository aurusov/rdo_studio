/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/plugin.cpp
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <QMessageBox>
#include <QDir>
#include <sstream>
#include <iostream>
#include <fstream>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_window.h"
#include "app/rdo_studio/src/model/model_tab_ctrl.h"
#include "app/rdo_studio/plugins/game5/src/plugin_game5_dialog.h"
#include "app/rdo_studio/plugins/game5/src/plugin_game5_tiles_order_dialog.h"
#include "app/rdo_studio/plugins/game5/src/costsetuptable.h"
// --------------------------------------------------------------------------------

game5Dialog::game5Dialog(QWidget * pParent)
	: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
	setupUi(this);
	hiddenWidget->setFixedWidth(280);
	hiddenWidget->hide();

	buttonHide->setCheckable(true);
	buttonHide->setDefault(false);

	setFixedHeight(std::max(375, gameBoard->m_boardSizeY + 160));
	adjustSize();
	connect(tableCostValue    ,SIGNAL(itemCheckStateChanged(QTableWidgetItem *)),this      ,SLOT(onItemCheckStateChanged(QTableWidgetItem *)));
	connect(buttonHide        ,SIGNAL(toggled(bool))                            ,this      ,SLOT(onClickHide(bool)));
	connect(buttonSetLineup   ,SIGNAL(clicked())                                ,this      ,SLOT(callDialog()));
	connect(buttonOk          ,SIGNAL(clicked())                                ,this      ,SLOT(onClickOk()));
	connect(buttonRandomLineup,SIGNAL(clicked())                                ,this      ,SLOT(emitSolvabilityCheck()));
	connect(buttonRightLineup ,SIGNAL(clicked())                                ,gameBoard ,SLOT(buildRightLineup()));
	connect(this              ,SIGNAL(buttonRandomClicked(bool))                ,gameBoard ,SLOT(buildRandomLineup(bool)));

	if (pParent) {
		move(pParent->frameGeometry().center() - frameGeometry().center());
	}
}

void game5Dialog::onClickHide(bool state)
{
	hiddenWidget->setVisible(state);
	buttonHide->setText(buttonHide->text() == "<<" ? ">>" : "<<");
	adjustSize();
}

void game5Dialog::onClickOk()
{
	MainWindow* pMainWindow = (MainWindow *)(parent());
	rdo::gui::model::Model* pModel = pMainWindow->getModel();
	if(pModel)
		if (pModel->getTab())
		{
			//pModel->saveModel(); !private
			//QString a = pModel->getFullName(); !RDORepository
			backUpModel(pModel);
			clearAllTabs(pModel);

			pModel->getTab()->getItemEdit(rdoModelObjects::RTP)->appendText(QString::fromStdString(RTPtabText()));
			pModel->getTab()->getItemEdit(rdoModelObjects::RSS)->appendText(QString::fromStdString(RSStabText()));
			pModel->getTab()->getItemEdit(rdoModelObjects::PAT)->appendText(QString::fromStdString(PATtabText()));
			pModel->getTab()->getItemEdit(rdoModelObjects::DPT)->appendText(QString::fromStdString(DPTtabText()));
			pModel->getTab()->getItemEdit(rdoModelObjects::FUN)->appendText(QString::fromStdString(FUNtabText()));

			//pModel->runModel();
		}
		else
			QMessageBox::warning(pMainWindow, "Игра 5",
			                    "Не найден текстовый редактор!\nВозможно не открыта ни одна модель.");
	else
		QMessageBox::warning(pMainWindow, "Игра 5",
		                     "Не найдена модель!\nВозможно не открыта ни одна модель.");
	done(Accepted);
}

void game5Dialog::emitSolvabilityCheck()
{
	emit buttonRandomClicked(solvabilityCheck->isChecked());
}

void game5Dialog::onItemCheckStateChanged(QTableWidgetItem * item)
{
	QTableWidget * parentTable = item->tableWidget();
	if (item->checkState() == Qt::Unchecked)
	{
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		item->setText("1");
	}
	else
	{
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		parentTable->edit(parentTable->model()->index(item->row(),item->column()));
	}
}

std::string game5Dialog::evaluateBy()
{
	if (radioButton0->isChecked())
		return "0";
	if (radioButtonQuantity->isChecked())
		return "Кол_во_фишек_не_на_месте()";
	if (radioButtonDistance->isChecked())
		return "Расстояния_фишек_до_мест()";
	if (radioButtonCustom->isChecked())
		return lineEditCustom->text().toStdString();
}

std::string game5Dialog::activityValue(int tableRow)
{
	QString string = tableCostValue->item(tableRow,1)->text() + " " + tableCostValue->item(tableRow,2)->text();
	return string.toStdString();
}

std::string game5Dialog::RTPtabText()
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

std::string game5Dialog::RSStabText()
{
	std::stringstream RSStabTextStream; 
	RSStabTextStream <<	"$Resources\n";
	for (unsigned int i = 1 ; i < gameBoard->getTilesPos().size() ; i++)
	{
		RSStabTextStream << "\tФишка Фишка" << i <<" = new Фишка(" << i << ", " << gameBoard->getTilesPos()[i] << ");\n";
	}
	RSStabTextStream << "\tДырка_t Дырка = new Дырка_t(" << gameBoard->getTilesPos()[0] << ");\n";
	RSStabTextStream <<	"$End\n";
	return RSStabTextStream.str();
}

std::string game5Dialog::PATtabText()
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

std::string game5Dialog::DPTtabText()
{
	std::stringstream DPTtabTextStream; 
	DPTtabTextStream
	<<	"$Decision_point Расстановка_фишек : search trace_all\n"
	<<	"$Condition Exist(Фишка: Фишка.Номер <> Фишка.Местоположение)\n"
	<<	"$Term_condition\n"
	<<	"	For_All(Фишка: Фишка.Номер == Фишка.Местоположение)\n"
	<<	"$Evaluate_by " << evaluateBy() << "\n"
	<<	"$Compare_tops = " << (radioButtonCopareTop->isChecked() ? "YES" : "NO") << "\n"
	<<	"$Activities\n"
	<<	"	Перемещение_вправо: Перемещение_фишки справа  1 value " << activityValue(0) << "\n"
	<<	"	Перемещение_влево : Перемещение_фишки слева  -1 value " << activityValue(1) << "\n"
	<<	"	Перемещение_вверх : Перемещение_фишки сверху -" << gameBoard->m_tilesCountX << " value " << activityValue(2) << "\n"
	<<	"	Перемещение_вниз  : Перемещение_фишки снизу   " << gameBoard->m_tilesCountX << " value " << activityValue(3) << "\n"
	<<	"$End";
	return DPTtabTextStream.str();
}

std::string game5Dialog::FUNtabText()
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
	for (unsigned int i=2;i < gameBoard->getTilesPos().size() - 1; i++)
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
	for (unsigned int i=2;i < gameBoard->getTilesPos().size() - 1; i++)
	{
		FUNtabTextStream
	<<	"	       Расстояние_фишки_до_места(Фишка" << i << ".Номер, Фишка" << i << ".Местоположение)+\n";
	}
	FUNtabTextStream
	<<	"	       Расстояние_фишки_до_места(Фишка" << gameBoard->getTilesPos().size() - 1 << ".Номер, Фишка" << gameBoard->getTilesPos().size() - 1 << ".Местоположение);\n"
	<<	"$End";
	return FUNtabTextStream.str();
}

void game5Dialog::backUpModel(rdo::gui::model::Model* pModel)
{
	QString backUpFolder = g_pApp->applicationDirPath() + "/model_backup/" + pModel->getName() + QDateTime::currentDateTime().toString("_yyyy-MM-dd_HH.mm.ss") + "/";
	QDir makeDir;
	bool success = makeDir.mkpath(backUpFolder);
	for (int i = 0; i < pModel->getTab()->tabBar()->count() ; i++)
	{
		if (!(pModel->getTab()->getItemEdit(i)->isEmpty()))
		{
			QString backUpFile = backUpFolder + pModel->getName() + "." + pModel->getTab()->tabBar()->tabText(i).toLower();
			std::ofstream File;
			File.open(backUpFile.toStdString().c_str());
			rdo::textstream txtStream;
			pModel->getTab()->getItemEdit(i)->save(txtStream);
			std::string tabStr = txtStream.str();
			File << tabStr;
			File.close();
		}
	}

	QString backUpFile = backUpFolder + pModel->getName() + "." + "rdox";
	std::ofstream File;
	File.open(backUpFile.toStdString().c_str());
	File
	<< "<?xml version=\"1.0\"?>\n"
	<< "<Settings>\n"
	<< "	<Version ProjectVersion=\"2\" SMRVersion=\"2\" />\n"
	<< "</Settings>";
	File.close();
}

void game5Dialog::clearAllTabs(rdo::gui::model::Model* pModel)
{
	for (int i = 0; i < pModel->getTab()->tabBar()->count() ; i++)
		pModel->getTab()->getItemEdit(i)->clearAll();
}

void game5Dialog::callDialog()
{
	TilesOrderDialog dlg(this, gameBoard->getTilesPos());
	connect(&dlg , SIGNAL(tilesOrderCommited(QString)) , gameBoard , SLOT(setTilesPositon(QString)));
	dlg.exec();
}
