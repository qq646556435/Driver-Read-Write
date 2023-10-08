#include "def.h"
PDEVICE_OBJECT DeviceObject;
UNICODE_STRING DeviceName, LineName;
NTSTATUS DispatchCreate(PDEVICE_OBJECT DriverObject, PIRP Irp);
NTSTATUS DispatchIoctl(PDEVICE_OBJECT DriverObject, PIRP Irp);
VOID DriverUnload(PDRIVER_OBJECT DriverObject);

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	UNREFERENCED_PARAMETER(RegistryPath);
	NTSTATUS Status = STATUS_SUCCESS;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = DispatchCreate;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchIoctl;
	DriverObject->DriverUnload = DriverUnload;
	RtlInitUnicodeString(&DeviceName, DEVICE_NAME);
	Status = IoCreateDevice(DriverObject, 0, &DeviceName, FILE_DEVICE_UNKNOWN, 0, FALSE, &DeviceObject);
	if (!NT_SUCCESS(Status)) return Status;
	RtlInitUnicodeString(&LineName, LINK_NAME);
	Status = IoCreateSymbolicLink(&LineName, &DeviceName);
	if (!NT_SUCCESS(Status))
	{
		IoDeleteDevice(DeviceObject);
		return Status;
	}
	PLDR_DATA_TABLE_ENTRY Ldr = (PLDR_DATA_TABLE_ENTRY)DriverObject->DriverSection;
	Ldr->Flags |= 0x20;
	Status = StartObCallBack();
	return Status;
}
NTSTATUS DispatchCreate(PDEVICE_OBJECT DriverObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DriverObject);
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}
NTSTATUS DispatchIoctl(PDEVICE_OBJECT DriverObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DriverObject);
	NTSTATUS Status = STATUS_SUCCESS;
	PIO_STACK_LOCATION  IoStackLocation = NULL;
	PDataStruct InputData = NULL, OutputData = NULL;
	ULONG OutputDataLength = 0, IoControlCode = 0;
	IoStackLocation = IoGetCurrentIrpStackLocation(Irp);
	IoControlCode = IoStackLocation->Parameters.DeviceIoControl.IoControlCode;
	InputData = Irp->AssociatedIrp.SystemBuffer;
	OutputData = Irp->AssociatedIrp.SystemBuffer;
	OutputDataLength = IoStackLocation->Parameters.DeviceIoControl.OutputBufferLength;
	switch (IoControlCode)
	{
	case IOCTL_IO_GetModuleBase:
	{
		ANSI_STRING AnsiBuffer = { 0 };
		UNICODE_STRING ModuleName = { 0 };
		AnsiBuffer.Buffer = ((PDataStruct)InputData)->Address;
		AnsiBuffer.Length = AnsiBuffer.MaximumLength = (USHORT)strlen(((PDataStruct)InputData)->Address);
		RtlAnsiStringToUnicodeString(&ModuleName, &AnsiBuffer, TRUE);
		Status = GetModuleBase((HANDLE)((PDataStruct)InputData)->ProcessId, ModuleName, OutputData);
		RtlFreeUnicodeString(&ModuleName);
		break;
	}
	case IOCTL_IO_ReadMemory:
	{
		Status = ReadMemory((HANDLE)InputData->ProcessId, InputData->Address, InputData->Size, OutputData);
		break;
	}
	case IOCTL_IO_WriteMemory:
	{
		Status = WriteMemory((HANDLE)InputData->ProcessId, InputData->Address, InputData->Size, InputData->Buffer);
		break;
	}
	case IOCTL_IO_SetProtectProcessId:
	{
		Status = SetProtectProcessId(*(PHANDLE)InputData);
		break;
	}
	default:
		Status = STATUS_UNSUCCESSFUL;
		break;
	}
	if (Status == STATUS_SUCCESS)
	{
		Irp->IoStatus.Information = OutputDataLength;
	}
	else
	{
		Irp->IoStatus.Information = 0;
	}
	Irp->IoStatus.Status = Status;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return Status;
}
VOID DriverUnload(PDRIVER_OBJECT DriverObject) {
	UNREFERENCED_PARAMETER(DriverObject);
	StopObCallBack();
	IoDeleteSymbolicLink(&LineName);
	IoDeleteDevice(DeviceObject);
}

