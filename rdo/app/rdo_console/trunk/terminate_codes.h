/*!
  \copyright (c) RDO-Team, 2011
  \file      app/rdo_console/terminate_codes.h
  \author    Пройдаков Евгений (lord.tiran@gmail.com)
  \date      26.10.2011
  \brief     Консольная версия RDO
  \indent    4T
*/

#ifndef _TERMINATE_CODES_H_
#define _TERMINATE_CODES_H_

#define TERMINATION_NORMAL                           0
#define TERMINATION_WITH_AN_ERROR_NO_MODEL           1
#define TERMINATION_WITH_AN_ERROR_OPEN_MODEL_ERROR   2
#define TERMINATION_WITH_AN_ERROR_PARSE_MODEL_ERROR  3
#define TERMINATION_WITH_AN_ERROR_RUNTIME_ERROR      4
#define TERMINATION_WITH_AN_ERROR_NO_EVENTS          5
#define TERMINATION_WITH_AN_ERROR_PARSE_EVENTS_ERROR 6
#define TERMINATION_WITH_AN_ERROR_CONVERTOR_ERROR    7

#define TERMINATION_WITH_APP_RUNTIME_ERROR           666

#endif // _TERMINATE_CODES_H_
