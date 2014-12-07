#ifndef _RDO_PLUGIN_GAME_5_MULTI_SELECT_COMPLETER_H_
#define _RDO_PLUGIN_GAME_5_MULTI_SELECT_COMPLETER_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QCompleter>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class MultiSelectCompleter : public QCompleter
{Q_OBJECT

public:
	MultiSelectCompleter(const QStringList& items, QObject* parent);
	~MultiSelectCompleter();

	virtual QString pathFromIndex(const QModelIndex& index) const;
	virtual QStringList splitPath(const QString& path) const;

signals:
	void changeSelection(int cursorPos, int selectionLen) const;

public slots:
	void onLineEditTextChanged();
	void setSelection(int cursorPos, int selectionLen);

private:
	int m_cursorPos;
	int m_selectionLen;
};

#endif // _RDO_PLUGIN_GAME_5_MULTI_SELECT_COMPLETER_H_
