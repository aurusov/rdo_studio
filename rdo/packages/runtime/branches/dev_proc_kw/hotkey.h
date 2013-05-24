/*!
  \copyright (c) RDO-Team, 2012
  \file      hotkey.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.01.2012
  \brief     
  \indent    4T
*/

#ifndef _LIB_RUNTIME_HOTKEY_H_
#define _LIB_RUNTIME_HOTKEY_H_

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- INCLUDES
#include <map>
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Горячие клавиши
class RDOHotKey
{
public:
	typedef  ruint               KeyCode;

private:
	typedef  std::list<KeyCode>  KeyList;

public:
	static const ruint UNDEFINED_KEY = ruint(~0);

	//! Вспомогательный класс
	class Toolkit
	{
	public:
		//! Конструктор, заполняет таблицу клавиш
		Toolkit();
		virtual ~Toolkit();

		//! Переводит имя клавиши в код клавиши
		//! @param  keyName - имя клавиши
		//! @result код клавиши в операционной системе
		KeyCode codeFromString(CREF(tstring) keyName) const;

	private:
		typedef  std::map<tstring, KeyCode>  KeySet;
		KeySet  m_keyList;
	};

	//! Используемые в моделе клавиши, список формируется в момент компиляции
	class KeyInModelList
	{
	public:
		//! Добавляет используемую в моделе клавишу
		//! @param  keyCode - код клавиши
		//! @result true, если код добавлен успешно
		rbool insert(CREF(KeyCode) keyCode);

		//! Проверяет наличие клавиши
		//! @param  keyCode - код клавиши
		//! @result true, если код найден в списке
		rbool check(CREF(KeyCode) keyCode) const;

	private:
		KeyList m_keyList;
	};

	//! Список текущих нажатых клавиш
	class KeyDownList
	{
	public:
		KeyDownList();
		virtual ~KeyDownList();

		//! Фиксирует нажатие клавиши
		//! @param  keyCode - код клавиши
		//! @result true, если клавиша добавлена в список нажатых
		rbool down(CREF(KeyCode) keyCode);

		//! Фиксирует отжатие клавиши
		//! @param  keyCode - код клавиши
		void up(CREF(KeyCode) keyCode);

		//! Проверяет, есть ли клавиша в списке нажатых
		//! @param  keyCode - код клавиши
		//! @param  shift   - признак нажатого Shift'а
		//! @param  control - признак нажатого Ctrl'а
		//! @result true, если клавиша есть в списке нажатых
		rbool isPressed(CREF(KeyCode) keyCode, rbool shift, rbool control);

		//! Результат других методов
		//! @result true, если \ref down или \ref isPressed вернули true в момент совего вызова
		rbool isFound() const;

		//! Очищает список текущих нажатых клавиш
		void clear();

	private:
		KeyList m_keyList;
		rbool   m_keyFound;
	};

	//! Обрабатывает клики мышкой на активные области, содержит только список текущих нажатых
	class AreaList
	{
	public:
		//! Фиксирует нажатие на область
		//! @param areaName - имя области
		void  click(CREF(tstring) areaName);

		//! Проверяет было ли нажатие на область
		//! @param  areaName - имя области
		//! @result true, если было нажатие
		rbool check(CREF(tstring) areaName);

		//! Проверяет было ли нажатие хотя бы на одну область
		//! @result true, если было нажатие
		rbool empty() const;

		//! Очищает список
		void clear();

	private:
		typedef  std::list<tstring>  NameList;
		NameList  m_activeAreasMouseClicked;
	};

	RDOHotKey();
	virtual ~RDOHotKey();

	//! Очищает нажатые клавиши и активные области
	void clear();

	//! Возвращает вспомогательный класс
	//! @result вспомогательный класс
	CREF(Toolkit) toolkit() const;

	//! Возвращает используемые в моделе клавиши
	//! @result используемые в моделе клавиши
	REF(KeyInModelList) keyInModel();

	//! Возвращает список текущих нажатых клавиш
	//! @result список текущих нажатых клавиш
	REF(KeyDownList) keyDown();

	//! Возвращает список текущий нажатых активных областей
	//! @result список текущий нажатых активных областей
	REF(AreaList) areaList();

	//! Проверяет факт нажатия клавиши или выбора активной области
	//! @result true, если была нажата клавиша или активная область
	rbool isKeyDown() const;

private:
	Toolkit        m_toolkit;
	KeyInModelList m_keyInModel;
	KeyDownList    m_keyDown;
	AreaList       m_areaList;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_HOTKEY_H_
