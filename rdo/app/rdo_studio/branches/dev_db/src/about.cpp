/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/about.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      18.08.2012
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/about.h"
#include "app/rdo_studio/res/build_version.h"
// --------------------------------------------------------------------------------

About::About(PTR(QWidget) pParent)
	: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
	setupUi(this);

	QString limitation;

#ifdef RDO_LICENSE_ACADEMIC
		limitation += QString::fromStdWString(L"не для коммерческого использования");
#endif

#ifdef RDO_LIMIT_RES
		if (!limitation.isEmpty())
			limitation += ", ";

		limitation += QString::fromStdWString(L"ограниченная версия");
#endif

	version->setText(QString::fromStdWString(L"Система имитационного моделирования\n\nRAO-studio   %1%2-version 1.1 (build %3)%4")

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
