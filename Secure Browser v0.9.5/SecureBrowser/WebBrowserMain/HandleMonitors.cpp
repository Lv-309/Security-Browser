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
		tlf_f << AT << ERROR_MIRRORING_DRIVER;
		FAILURE_BOX(ERROR_MIRRORING_DRIVER);
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
		tlf_f << AT << ERROR_NO_MONITORS_L;
		FAILURE_BOX(ERROR_NO_MONITORS_B);
		return ErrorTypes::NO_MONITORS;
	}
	else if (m_monitors.size() >= 2)
	{
		tlf_f << AT << ERROR_FEW_MONITORS_L;
		FAILURE_BOX(ERROR_FEW_MONITORS_B);
		return ErrorTypes::FEW_MONITORS;
	}
	tlf_i << AT << INFO_SUCCES_MONITORS;
	return ErrorTypes::IS_OK;
}