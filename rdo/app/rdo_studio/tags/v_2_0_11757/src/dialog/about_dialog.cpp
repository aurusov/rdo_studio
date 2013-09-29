/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      about_dialog.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      18.08.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dialog/about_dialog.h"
#include "app/rdo_studio/res/build_version.h"
// --------------------------------------------------------------------------------

AboutDialog::AboutDialog(QWidget* pParent)
	: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
	setupUi(this);

	QString limitation;

#ifdef RDO_LICENSE_ACADEMIC
		limitation += "не для коммерческого использования";
#endif

#ifdef RDO_LIMIT_RES
		if (!limitation.isEmpty())
			limitation += ", ";

		limitation += "ограниченная версия";
#endif

	version->setText(QString("Система имитационного моделирования\n\nRAO-studio   %1%2-%3 (build %4)%5")

#ifdef RDO_MT
		.arg("mt")
#else
		.arg("st")
#endif

#ifdef RDOSIM_COMPATIBLE
		.arg("-comp")
#else
		.arg("")
#endif

		.arg(version->text())

		.arg(g_buildVersion)

		.arg(!limitation.isEmpty()
			? "\n(" + limitation + ")"
			: ""
		)
	);

	if (pParent)
	{
		move(pParent->frameGeometry().center() - frameGeometry().center());
	}
}
