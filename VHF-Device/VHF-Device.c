#include "VHF-Device.h"

UCHAR HIDReportDescriptor[] = {
	0x05, 0x01,						// Usage Page (Generic Desktop)
	0x09, 0x04,						// Usage (Joystick)
	0xA1, 0x01,						// Collection (Application)

	// X and Y Axis
	0x05, 0x01,						// Usage Page (Generic Desktop)
	0x09, 0x30,						// Usage (X)
	0x09, 0x31,						// Usage (Y)
	0x15, 0x00,						// Logical Minimum (0)
	0x26, 0xFF, 0x00,				// Logical Maximum (255)
	0x75, 0x08,						// Report Size (8)
	0x95, 0x02,						// Report Count (2)
	0x81, 0x02,						// Input (Data, Variable, Absolute)

	// Z Axis
	0x09, 0x32,						// Usage (Z)
	0x15, 0x00,						// Logical Minimum (0)
	0x26, 0xFF, 0x00,				// Logical Maximum (255)
	0x75, 0x08,						// Report Size (8)
	0x95, 0x01,						// Report Count (1)
	0x81, 0x02,						// Input (Data, Variable, Absolute)

	// Rx and Ry Axes
	0x09, 0x33,						// Usage (Rx)
	0x09, 0x34,						// Usage (Ry)
	0x15, 0x00,						// Logical Minimum (0)
	0x26, 0xFF, 0x00,				// Logical Maximum (255)
	0x75, 0x08,						// Report Size (8)
	0x95, 0x02,						// Report Count (2)
	0x81, 0x02,						// Input (Data, Variable, Absolute)

	// Hat Switch (POV)
	0x05, 0x01,						// Usage Page (Generic Desktop)
	0x09, 0x39,						// Usage (Hat Switch)
	0x15, 0x00,						// Logical Minimum (0)
	0x25, 0x07,						// Logical Maximum (7) - 8 directions
	0x35, 0x00,						// Physical Minimum (0)
	0x46, 0x3B, 0x01,				// Physical Maximum (315) - 45 degrees per direction
	0x75, 0x04,						// Report Size (4)
	0x95, 0x01,						// Report Count (1)
	0x65, 0x14,						// Unit (Degrees, English Rotation)
	0x81, 0x02,						// Input (Data, Variable, Absolute)

	// Padding to align with 8-bit boundary
	0x75, 0x04,						// Report Size (4)
	0x95, 0x01,						// Report Count (1)
	0x81, 0x03,						// Input (Constant, Variable, Absolute) - Padding

	// 16 Buttons
	0x05, 0x09,						// Usage Page (Button)
	0x19, 0x01,						// Usage Minimum (Button 1)
	0x29, 0x10,						// Usage Maximum (Button 16)
	0x15, 0x00,						// Logical Minimum (0)
	0x25, 0x01,						// Logical Maximum (1)
	0x75, 0x01,						// Report Size (1)
	0x95, 0x10,						// Report Count (16)
	0x81, 0x02,						// Input (Data, Variable, Absolute)

	0xC0							// End Collection
};

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT  DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	WDF_DRIVER_CONFIG config;
	NTSTATUS status;

	KdPrint(("VHF-Device: Entering DriverEntry\n"));

	WDF_DRIVER_CONFIG_INIT(&config, EvtDriverDeviceAdd);
	config.EvtDriverUnload = EvtDriverUnload;

	KdPrint(("VHF-Device: Initialized driver config\n"));

	status = WdfDriverCreate(DriverObject, RegistryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, WDF_NO_HANDLE);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("VHF-Device: WdfDriverCreate failed with status: 0x%08x\n", status));
		return status;
	}

	KdPrint(("VHF-Device: WdfDriverCreate succeeded\n"));
	return status;
}

NTSTATUS EvtDriverDeviceAdd(_In_ WDFDRIVER Driver, _Inout_ PWDFDEVICE_INIT DeviceInit)
{
	UNREFERENCED_PARAMETER(Driver);
	WDF_OBJECT_ATTRIBUTES deviceAttributes;
	WDFDEVICE device;
	NTSTATUS status;
	PDEVICE_CONTEXT deviceContext;
	VHF_CONFIG vhfConfig;
	PDEVICE_OBJECT pdo;

	USHORT HIDReportDescriptorSize = sizeof(HIDReportDescriptor);

	KdPrint(("VHF-Device: Entering EvtDriverDeviceAdd\n"));

	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, DEVICE_CONTEXT);
	deviceAttributes.EvtCleanupCallback = EvtCleanupCallback;

	KdPrint(("VHF-Device: Creating device\n"));
	status = WdfDeviceCreate(&DeviceInit, &deviceAttributes, &device);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("VHF-Device: WdfDeviceCreate failed with status: 0x%08x\n", status));
		return status;
	}

	deviceContext = DeviceGetContext(device);
	RtlZeroMemory(deviceContext, sizeof(DEVICE_CONTEXT));
	KdPrint(("VHF-Device: Device context initialized\n"));

	pdo = WdfDeviceWdmGetDeviceObject(device);
	KdPrint(("VHF-Device: Device Object Pointer: %p\n", pdo));
	if (pdo == NULL)
	{
		KdPrint(("VHF-Device: Invalid Device Object Pointer\n"));
		return STATUS_INVALID_PARAMETER;
	}

	KdPrint(("VHF-Device: Report Descriptor Size: %lu\n", HIDReportDescriptorSize));

	VHF_CONFIG_INIT(&vhfConfig, pdo, HIDReportDescriptorSize, HIDReportDescriptor);
	KdPrint(("VHF-Device: VHF configuration initialized\n"));

	status = VhfCreate(&vhfConfig, &deviceContext->VhfHandle);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("VHF-Device: VhfCreate failed with status: 0x%08x\n", status));
		return status;
	}

	KdPrint(("VHF-Device: VhfCreate succeeded\n"));

	status = VhfStart(deviceContext->VhfHandle);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("VHF-Device: VhfStart failed with status: 0x%08x\n", status));
		return status;
	}

	KdPrint(("VHF-Device: VhfStart succeeded\n"));
	return status;
}

VOID EvtDriverUnload(_In_ WDFDRIVER Driver)
{
	UNREFERENCED_PARAMETER(Driver);
	KdPrint(("VHF-Device: Unloading\n"));
}

VOID EvtCleanupCallback(_In_ WDFOBJECT DeviceObject)
{
	PDEVICE_CONTEXT deviceContext = DeviceGetContext(DeviceObject);
	KdPrint(("VHF-Device: Cleanup started\n"));

	if (deviceContext->VhfHandle != WDF_NO_HANDLE)
	{
		KdPrint(("VHF-Device: Deleting VhfHandle\n"));
		VhfDelete(deviceContext->VhfHandle, TRUE);
	}

	KdPrint(("VHF-Device: Cleanup completed\n"));
}