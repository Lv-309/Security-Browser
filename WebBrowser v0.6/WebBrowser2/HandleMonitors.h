#pragma once
#include "stdafx.h"
#include "HelpTools.h"

namespace ISXHandleMonitors
{
	#define HANDLE_NOMON  0
	#define HANDLE_DONE   1
	#define MIRROR_DRIVER 2

	class HandleMonitors
	{
	public:
		typedef std::vector<RECT> MONITORLIST;
		typedef HandleMonitors*   LPHMONITORS;
	// Ctors
	public:
		 HandleMonitors();
		~HandleMonitors() = default;
	// Self-use only
	protected:
		VOID Verify() const noexcept;
		// static CALLBACK method to get monitors
		static BOOL CALLBACK MonitorEnum(HMONITOR h_mon, HDC hdc, LPRECT lprc_mon, LPARAM p_data);
	// Getter
	public:
		MONITORLIST GetMonitors() const noexcept;
	// List of connected monitors
	private:
		MONITORLIST		m_monitors;
	};

}
