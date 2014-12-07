// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/application_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/format.hpp>
#include <boost/algorithm/string/erase.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dialog/about_dialog.h"
#include "build_version.h"
// --------------------------------------------------------------------------------

AboutDialog::AboutDialog(QWidget* pParent)
	: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
	setupUi(this);

	version->setText(QString("Система имитационного моделирования\n\n%1")
		.arg(QString::fromStdString(rdo::version::g_versionName)));

	www->setText(QString::fromStdString(boost::str(
		boost::format("<A HREF=\"%1%\">%2%</A>")
		% rdo::version::g_site
		% boost::algorithm::erase_first_copy(rdo::version::g_site, "http://")
	)));

	if (pParent)
	{
		move(pParent->frameGeometry().center() - frameGeometry().center());
	}
}
