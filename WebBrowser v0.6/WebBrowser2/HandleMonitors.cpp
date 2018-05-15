#include "stdafx.h"
#include "HandleMonitors.h"
using namespace ISXHandleMonitors;

HandleMonitors::HandleMonitors()
{
	EnumDisplayMonitors(NULL, NULL, this->MonitorEnum, (LPARAM)this);
	this->Verify();
}

BOOL CALLBACK HandleMonitors::MonitorEnum(HMONITOR h_mon, HDC hdc, LPRECT lprc_mon, LPARAM p_data)
{
	MONITORINFO	monitor_info;
	monitor_info.cbSize = sizeof(monitor_info);
	GetMonitorInfo(h_mon, &monitor_info);
	if (monitor_info.dwFlags == DISPLAY_DEVICE_MIRRORING_DRIVER)
	{
		FAILURE_BOX("mirror application");
		// TODO: Throw error a pseudo device used to mirror application 
		// drawing for remoting or other purposes.
		return MIRROR_DRIVER;
	}
	LPHMONITORS h_self = (LPHMONITORS)p_data;
	h_self->m_monitors.push_back(*lprc_mon);
	return HANDLE_DONE;
}

HandleMonitors::MONITORLIST HandleMonitors::GetMonitors() const noexcept
{
	return this->m_monitors;
}

VOID HandleMonitors::Verify() const noexcept
{
	if (m_monitors.size() == 0)
	{
		FAILURE_BOX("No connected monitors");
		// TODO: Throw error
		exit(EXIT_SUCCESS);
	}
	else if (m_monitors.size() >= 2)
	{
		FAILURE_BOX("You have more than 1 connected monitors");
		// TODO: Throw error
		exit(EXIT_SUCCESS);
	}
}