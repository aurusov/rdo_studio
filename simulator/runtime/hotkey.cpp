// ----------------------------------------------------------------------- PLATFORM
// ----------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/range/algorithm/find.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/hotkey.h"
#include "simulator/runtime/keyboard.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOHotKey::Toolkit
// --------------------------------------------------------------------------------
RDOHotKey::Toolkit::Toolkit()
{
    m_keyList.insert(KeySet::value_type("ESCAPE",   VK_ESCAPE));
    m_keyList.insert(KeySet::value_type("TAB",      VK_TAB));
    m_keyList.insert(KeySet::value_type("SHIFT",    VK_SHIFT));
    m_keyList.insert(KeySet::value_type("CONTROL",  VK_CONTROL));
    m_keyList.insert(KeySet::value_type("BACK",     VK_BACK));
    m_keyList.insert(KeySet::value_type("RETURN",   VK_RETURN));
    m_keyList.insert(KeySet::value_type("INSERT",   VK_INSERT));
    m_keyList.insert(KeySet::value_type("HOME",     VK_HOME));
    m_keyList.insert(KeySet::value_type("PRIOR",    VK_PRIOR));
    m_keyList.insert(KeySet::value_type("DELETE",   VK_DELETE));
    m_keyList.insert(KeySet::value_type("END",      VK_END));
    m_keyList.insert(KeySet::value_type("NEXT",     VK_NEXT));
    m_keyList.insert(KeySet::value_type("UP",       VK_UP));
    m_keyList.insert(KeySet::value_type("LEFT",     VK_LEFT));
    m_keyList.insert(KeySet::value_type("DOWN",     VK_DOWN));
    m_keyList.insert(KeySet::value_type("RIGHT",    VK_RIGHT));
    m_keyList.insert(KeySet::value_type("DIVIDE",   VK_DIVIDE));
    m_keyList.insert(KeySet::value_type("MULTIPLY", VK_MULTIPLY));
    m_keyList.insert(KeySet::value_type("SUBTRACT", VK_SUBTRACT));
    m_keyList.insert(KeySet::value_type("ADD",      VK_ADD));
    m_keyList.insert(KeySet::value_type("CLEAR",    VK_CLEAR));
    m_keyList.insert(KeySet::value_type("SPACE",    VK_SPACE));
    m_keyList.insert(KeySet::value_type("DECIMAL",  VK_DECIMAL));
    m_keyList.insert(KeySet::value_type("F2",       VK_F2));
    m_keyList.insert(KeySet::value_type("F3",       VK_F3));
    m_keyList.insert(KeySet::value_type("F4",       VK_F4));
    m_keyList.insert(KeySet::value_type("F5",       VK_F5));
    m_keyList.insert(KeySet::value_type("F6",       VK_F6));
    m_keyList.insert(KeySet::value_type("F7",       VK_F7));
    m_keyList.insert(KeySet::value_type("F8",       VK_F8));
    m_keyList.insert(KeySet::value_type("F9",       VK_F9));
    m_keyList.insert(KeySet::value_type("F10",      VK_F10));
    m_keyList.insert(KeySet::value_type("F11",      VK_F11));
    m_keyList.insert(KeySet::value_type("F12",      VK_F12));
    m_keyList.insert(KeySet::value_type("NUMPAD0",  VK_NUMPAD0));
    m_keyList.insert(KeySet::value_type("NUMPAD1",  VK_NUMPAD1));
    m_keyList.insert(KeySet::value_type("NUMPAD2",  VK_NUMPAD2));
    m_keyList.insert(KeySet::value_type("NUMPAD3",  VK_NUMPAD3));
    m_keyList.insert(KeySet::value_type("NUMPAD4",  VK_NUMPAD4));
    m_keyList.insert(KeySet::value_type("NUMPAD5",  VK_NUMPAD5));
    m_keyList.insert(KeySet::value_type("NUMPAD6",  VK_NUMPAD6));
    m_keyList.insert(KeySet::value_type("NUMPAD7",  VK_NUMPAD7));
    m_keyList.insert(KeySet::value_type("NUMPAD8",  VK_NUMPAD8));
    m_keyList.insert(KeySet::value_type("NUMPAD9",  VK_NUMPAD9));
    m_keyList.insert(KeySet::value_type("NOKEY",    0));

    for (char i = '0'; i <= '9'; ++i)
    {
        m_keyList.insert(KeySet::value_type(std::string(1, i), (KeyCode)i));
    }

    for (char i = 'A'; i <= 'Z'; ++i)
    {
        m_keyList.insert(KeySet::value_type(std::string(1, i), (KeyCode)i));
    }
}

RDOHotKey::Toolkit::~Toolkit()
{}

RDOHotKey::KeyCode RDOHotKey::Toolkit::codeFromString(const std::string& keyName) const
{
    KeySet::const_iterator it = m_keyList.find(keyName);
    return it == m_keyList.end() ? KeyCode(UNDEFINED_KEY) : it->second;
}

// --------------------------------------------------------------------------------
// -------------------- RDOHotKey::KeyInModelList
// --------------------------------------------------------------------------------
bool RDOHotKey::KeyInModelList::insert(const KeyCode& keyCode)
{
    if (check(keyCode))
    {
        return false;
    }
    m_keyList.push_back(keyCode);
    return true;
}

bool RDOHotKey::KeyInModelList::check(const KeyCode& keyCode) const
{
    return boost::find(m_keyList, keyCode) != m_keyList.end();
}

// --------------------------------------------------------------------------------
// -------------------- RDOHotKey::KeyDownList
// --------------------------------------------------------------------------------
RDOHotKey::KeyDownList::KeyDownList()
    : m_keyFound(false)
{}

RDOHotKey::KeyDownList::~KeyDownList()
{}

bool RDOHotKey::KeyDownList::down(const KeyCode& keyCode)
{
    // Если нажаты VK_SHIFT или VK_CONTROL, то сбросим буфер клавиатуры
    if (keyCode == VK_SHIFT || keyCode == VK_CONTROL)
    {
        KeyList::iterator it = m_keyList.begin();
        while (it != m_keyList.end())
        {
            if (*it != VK_SHIFT && *it != VK_CONTROL)
            {
                it = m_keyList.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
    // Подсчитаем сколько раз клавиша уже в буфере
    int cnt = 0;
    KeyList::iterator it = m_keyList.begin();
    while (it != m_keyList.end())
    {
        if (*it == keyCode)
        {
            ++cnt;
        }
        ++it;
    }
    // Добавим клавишу в буфер
    if (cnt < 4)
    {
        m_keyList.push_back(keyCode);
    }
    if (cnt == 0) m_keyFound = true;
    return cnt > 0;
}

void RDOHotKey::KeyDownList::up(const KeyCode& keyCode)
{
    // Если отжаты VK_SHIFT или VK_CONTROL, то сбросим удалим их из буфера
    //if (keyCode == VK_SHIFT || keyCode == VK_CONTROL)
    //{
        KeyList::iterator it = m_keyList.begin();
        while (it != m_keyList.end())
        {
            if (*it == keyCode)
            {
                it = m_keyList.erase(it);
            }
            else
            {
                ++it;
            }
        }
    //}
}

bool RDOHotKey::KeyDownList::isPressed(const KeyCode& keyCode, bool shift, bool control)
{
    if (keyCode == 0) return false;
    bool shift_found   = false;
    bool control_found = false;
    // Найдем VK_SHIFT и/или VK_CONTROL в буфере
    KeyList::iterator it = m_keyList.begin();
    while (it != m_keyList.end())
    {
        if (*it == VK_SHIFT)
        {
            shift_found = true;
            if (shift_found && control_found) break;
        }
        if (*it == VK_CONTROL)
        {
            control_found = true;
            if (shift_found && control_found) break;
        }
        ++it;
    }
    // Теперь найдем саму клавишу в буфере
    // Удалим её из буфера перед выходом
    if (shift_found == shift && control_found == control)
    {
        KeyList::iterator it = m_keyList.begin();
        while (it != m_keyList.end())
        {
            if (*it == keyCode)
            {
                m_keyList.erase(it);
                m_keyFound = true;
                return true;
            }
            ++it;
        }
    }
    m_keyFound = false;
    return false;
}

bool RDOHotKey::KeyDownList::isFound() const
{
    return m_keyFound;
}

void RDOHotKey::KeyDownList::clear()
{
    m_keyList.clear();
}

// --------------------------------------------------------------------------------
// -------------------- RDOHotKey::AreaList
// --------------------------------------------------------------------------------
void RDOHotKey::AreaList::click(const std::string& areaName)
{
    if (boost::find(m_activeAreasMouseClicked, areaName) != m_activeAreasMouseClicked.end())
        return;

    m_activeAreasMouseClicked.push_back(areaName);
}

bool RDOHotKey::AreaList::check(const std::string& areaName)
{
    NameList::iterator it = boost::find(m_activeAreasMouseClicked, areaName);
    if (it == m_activeAreasMouseClicked.end())
    {
        return false;
    }
    m_activeAreasMouseClicked.erase(it);
    return true;
}

bool RDOHotKey::AreaList::empty() const
{
    return m_activeAreasMouseClicked.empty();
}

void RDOHotKey::AreaList::clear()
{
    m_activeAreasMouseClicked.clear();
}

// --------------------------------------------------------------------------------
// -------------------- RDOHotKey
// --------------------------------------------------------------------------------
RDOHotKey::RDOHotKey()
{}

RDOHotKey::~RDOHotKey()
{}

void RDOHotKey::clear()
{
    m_keyDown .clear();
    m_areaList.clear();
}

bool RDOHotKey::isKeyDown() const
{
    return m_keyDown.isFound() || !m_areaList.empty();
}

const RDOHotKey::Toolkit& RDOHotKey::toolkit() const
{
    return m_toolkit;
}

RDOHotKey::KeyInModelList& RDOHotKey::keyInModel()
{
    return m_keyInModel;
}

RDOHotKey::KeyDownList& RDOHotKey::keyDown()
{
    return m_keyDown;
}

RDOHotKey::AreaList& RDOHotKey::areaList()
{
    return m_areaList;
}

CLOSE_RDO_RUNTIME_NAMESPACE
