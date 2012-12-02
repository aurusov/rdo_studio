/*!
  \copyright (c) RDO-Team, 2012
  \file      app/rdo_console/rdo_event.h
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.11.2012
  \brief     Консольная версия RDO
  \indent    4T
*/

#ifndef _RDO_EVENT_H_
#define _RDO_EVENT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

/// base event class
class event
{
public:
    enum types {
        none,
        key,
        mouse
    };

public:
    event(CREF(tstring) name, double time, types type = none);
    virtual ~event();

    void setName(CREF(tstring) name);
    void setTime(double time);

    tstring getName() const;
    double  getTime() const;
    types   getType() const;

    friend bool operator< (CREF(event) left, CREF(event) right);

private:
	tstring m_name;
	double  m_time;
    types   m_type;
};

CLOSE_RDO_NAMESPACE

#endif // _RDO_EVENT_H_