#include "stdafx.h"
#include "HelpTools.h"
#include "HandleMonitors.h"
#include "..\TraceUtility\LogInfo.h"
#include "..\TraceUtility\LogFatal.h"
using namespace ISXHandleMonitors;

HandleMonitors::HandleMonitors()
{
	EnumDisplayMonitors(NULL, NULL, this->MonitorEnum, (LPARAM)this);
}

BOOL CALLBACK HandleMonitors::MonitorEnum(HMONITOR h_mon, HDC hdc, LPRECT lprc_mon, LPARAM p_data)
{
	MONITORINFO	monitor_info;
	monitor_info.cbSize = sizeof(monitor_info);
	GetMonitorInfo(h_mon, &monitor_info);
	if (monitor_info.dwFlags == DISPLAY_DEVICE_MIRRORING_DRIVER)
	{
		tlf_f << AT << "A pseudo device used to mirror application drawing for remoting or other purposes";
		FAILURE_BOX("A pseudo device used to mirror application drawing for remoting or other purposes");
		return (BOOL)ErrorTypes::MONITOR_MIRROR_DRIVER;
	}
	HandleMonitors* h_self = (HandleMonitors*)p_data;
	h_self->m_monitors.push_back(*lprc_mon);
	return (BOOL)ErrorTypes::IS_OK;
}

HandleMonitors::MonitorsList HandleMonitors::GetMonitors() const noexcept
{
	return this->m_monitors;
}

ErrorTypes HandleMonitors::Verify() const noexcept
{
	if (m_monitors.size() == 0)
	{
		tlf_f << AT << "User haven't any connected monitors";
		FAILURE_BOX("You haven't any connected monitors");
		return ErrorTypes::NO_MONITORS;
	}
	else if (m_monitors.size() >= 2)
	{
		tlf_f << AT << "User have more than 1 connected monitors";
		FAILURE_BOX("You have more than 1 connected monitors");
		return ErrorTypes::FEW_MONITORS;
	}
	tlf_i << AT << "Successful user verification on connected monitors";
	return ErrorTypes::IS_OK;
}