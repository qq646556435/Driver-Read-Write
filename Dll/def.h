#pragma once
#include <Windows.h>
#define IOCTL_IO_GetModuleBase                 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_IO_ReadMemory                    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_IO_WriteMemory                   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_IO_SetProtectProcessId           CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)
typedef struct _DataStruct {
	ULONG	ProcessId;
	ULONG64	Address;
	ULONG	Size;
	ULONG64	Buffer;
} DataStruct, * PDataStruct;
extern "C" __declspec(dllexport) BOOL GetDeviceHandle();
extern "C" __declspec(dllexport) BOOL GetModuleBase(ULONG ProcessId, PCHAR ModuleName, PULONG64 ModuleBase);
extern "C" __declspec(dllexport) BOOL ReadMemory(ULONG ProcessId, PULONG64 Address, ULONG Size, PULONG64 Buffer);
extern "C" __declspec(dllexport) BOOL WriteMemory(ULONG ProcessId, PULONG64 Address, ULONG Size, PULONG64 Buffer);
extern "C" __declspec(dllexport) BOOL SetProtectProcessId(ULONG ProcessId);
