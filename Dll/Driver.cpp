#include "def.h"
HANDLE hDevice = INVALID_HANDLE_VALUE;
//获取驱动句柄
BOOL GetDeviceHandle() {
	hDevice = CreateFileA("\\\\.\\MemoryTest", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);
	if (hDevice == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	else {
	    return TRUE;
	}
}
//获取模块基址
//参数1：进程ID
//参数2：模块名（易语言中为文本型指针）
//参数3：返回的模块基址（易语言中为长整数型的指针）
//返回值：逻辑型
BOOL GetModuleBase(ULONG ProcessId, PCHAR ModuleName, PULONG64 ModuleBase) {
	if (hDevice == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	DataStruct Data;
	Data.ProcessId = ProcessId;
	Data.Address = (ULONG64)ModuleName;
	Data.Size = strlen(ModuleName);
	Data.Buffer = (ULONG64)ModuleBase;
	DWORD dwRet = 0;
	BOOL bRet = DeviceIoControl(hDevice, IOCTL_IO_GetModuleBase, &Data, sizeof(Data), ModuleBase, sizeof(PULONG64), &dwRet, 0);
	if (bRet == FALSE) {
		return FALSE;
	}
	else {
		return TRUE;
	}
}
//读取内存
//参数1：进程ID（易语言中为整数型）
//参数2：要读取的内存地址（易语言中为长整数型的指针）
//参数3：要读取的内存大小（易语言中为整数型）
//参数4：返回的内存数据（易语言中为长整数型的指针）
//返回值：逻辑型
BOOL ReadMemory(ULONG ProcessId, PULONG64 Address, ULONG Size, PULONG64 Buffer) {
	if (hDevice == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	DataStruct Data;
	Data.ProcessId = ProcessId;
	Data.Address = (ULONG64)Address;
	Data.Size = Size;
	Data.Buffer = (ULONG64)Buffer;
	DWORD dwRet = 0;
	BOOL bRet = DeviceIoControl(hDevice, IOCTL_IO_ReadMemory, &Data, sizeof(Data), &Data, sizeof(Data), &dwRet, 0);
	if (bRet == FALSE) {
		return FALSE;
	}
	else {
		return TRUE;
	}
}
//写入内存
//参数1：进程ID（易语言中为整数型）
//参数2：要写入的内存地址（易语言中为长整数型的指针）
//参数3：要写入的内存大小（易语言中为整数型）
//参数4：要写入的内存数据（易语言中为长整数型的指针）
//返回值：逻辑型
BOOL WriteMemory(ULONG ProcessId, PULONG64 Address, ULONG Size, PULONG64 Buffer) {
	if (hDevice == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	DataStruct Data;
	Data.ProcessId = ProcessId;
	Data.Address = (ULONG64)Address;
	Data.Size = Size;
	Data.Buffer = (ULONG64)Buffer;
	DWORD dwRet = 0;
	BOOL bRet = DeviceIoControl(hDevice, IOCTL_IO_WriteMemory, &Data, sizeof(Data), &Data, sizeof(Data), &dwRet, 0);
	if (bRet == FALSE) {
		return FALSE;
	}
	else {
		return TRUE;
	}
}
//设置保护进程ID
//参数1：进程ID（易语言中为整数型）
//返回值：逻辑型
BOOL SetProtectProcessId(ULONG ProcessId) {
	if (hDevice == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	DWORD dwRet = 0;
	BOOL bRet = DeviceIoControl(hDevice, IOCTL_IO_SetProtectProcessId, &ProcessId, sizeof(ULONG), 0, 0, &dwRet, 0);
	if (bRet == FALSE) {
		return FALSE;
	}
	else {
		return TRUE;
	}
}