#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_console/events/rdo_event.h"
// --------------------------------------------------------------------------------

namespace rdo {

class mouse_event: public event
{
public:
	enum buttons
	{
		none,
		left,
		right,
		middle
	};

	mouse_event(const std::string& name, double time, buttons button, int x, int y);
	virtual ~mouse_event();

	void setButton(buttons button);
	void setX(int x);
	void sety(int y);

	buttons getButton() const;
	int getX() const;
	int getY() const;

private:
	buttons m_button;

	int m_x;
	int m_y;
};

} // namespace rdo
