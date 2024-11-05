#pragma once
#include <ntddk.h>
#include <wdf.h>
#include <vhf.h>

typedef struct _DEVICE_CONTEXT
{
	VHFHANDLE VhfHandle;
} DEVICE_CONTEXT, * PDEVICE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, DeviceGetContext)

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD EvtDriverDeviceAdd;
EVT_WDF_DRIVER_UNLOAD EvtDriverUnload;
EVT_WDF_OBJECT_CONTEXT_CLEANUP EvtCleanupCallback;
