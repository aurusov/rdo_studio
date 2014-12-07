#pragma once

// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- INCLUDES
#include <map>
#include <list>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "simulator/runtime/calc/calc_base.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

class RDOHotKey
{
public:
	typedef std::size_t KeyCode;

private:
	typedef  std::list<KeyCode>  KeyList;

public:
	static const std::size_t UNDEFINED_KEY = std::size_t(~0);

	class Toolkit
	{
	public:
		Toolkit();
		virtual ~Toolkit();

		KeyCode codeFromString(const std::string& keyName) const;

	private:
		typedef std::map<std::string, KeyCode> KeySet;
		KeySet m_keyList;
	};

	class KeyInModelList
	{
	public:
		bool insert(const KeyCode& keyCode);

		bool check(const KeyCode& keyCode) const;

	private:
		KeyList m_keyList;
	};

	class KeyDownList
	{
	public:
		KeyDownList();
		virtual ~KeyDownList();

		bool down(const KeyCode& keyCode);
		void up(const KeyCode& keyCode);

		bool isPressed(const KeyCode& keyCode, bool shift, bool control);
		bool isFound() const;
		void clear();

	private:
		KeyList m_keyList;
		bool m_keyFound;
	};

	class AreaList
	{
	public:
		void  click(const std::string& areaName);

		bool check(const std::string& areaName);
		bool empty() const;
		void clear();

	private:
		typedef std::list<std::string> NameList;
		NameList m_activeAreasMouseClicked;
	};

	RDOHotKey();
	virtual ~RDOHotKey();

	void clear();

	const Toolkit& toolkit() const;
	KeyInModelList& keyInModel();
	KeyDownList& keyDown();
	AreaList& areaList();

	bool isKeyDown() const;

private:
	Toolkit m_toolkit;
	KeyInModelList m_keyInModel;
	KeyDownList m_keyDown;
	AreaList m_areaList;
};

CLOSE_RDO_RUNTIME_NAMESPACE
