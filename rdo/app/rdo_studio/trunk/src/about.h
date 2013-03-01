/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/about.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      18.08.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_ABOUT_H_
#define _RDO_STUDIO_ABOUT_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QDialog>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/projects/common/bin/rdo_studio/generated/ui_about.h"
// --------------------------------------------------------------------------------

class About: public QDialog, private Ui::AboutDialog
{
Q_OBJECT

public:
	explicit About(PTR(QWidget) pParent = NULL);
};

#endif // _RDO_STUDIO_ABOUT_H_
