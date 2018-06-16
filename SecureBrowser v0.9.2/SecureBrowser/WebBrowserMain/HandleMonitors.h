#pragma once
#include "stdafx.h"
#include "ErrorTypes.h"

namespace ISXHandleMonitors
{
	class HandleMonitors
	{
	public:
		typedef std::vector<RECT> MonitorsList;
	// Ctors
	public:
		 HandleMonitors();
		~HandleMonitors() = default;
	// static CALLBACK method to get monitors
	protected:
		static BOOL CALLBACK MonitorEnum(HMONITOR h_mon, HDC hdc, LPRECT lprc_mon, LPARAM p_data);
	// Controls
	public:
		ErrorTypes   Verify()		const noexcept;
		MonitorsList GetMonitors()	const noexcept;
	// List of connected monitors
	private:
		MonitorsList		m_monitors;
	};

}
