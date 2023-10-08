#include "def.h"
HANDLE hDevice = INVALID_HANDLE_VALUE;
//��ȡ�������
BOOL GetDeviceHandle() {
	hDevice = CreateFileA("\\\\.\\MemoryTest", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);
	if (hDevice == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	else {
	    return TRUE;
	}
}
//��ȡģ���ַ
//����1������ID
//����2��ģ��������������Ϊ�ı���ָ�룩
//����3�����ص�ģ���ַ����������Ϊ�������͵�ָ�룩
//����ֵ���߼���
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
//��ȡ�ڴ�
//����1������ID����������Ϊ�����ͣ�
//����2��Ҫ��ȡ���ڴ��ַ����������Ϊ�������͵�ָ�룩
//����3��Ҫ��ȡ���ڴ��С����������Ϊ�����ͣ�
//����4�����ص��ڴ����ݣ���������Ϊ�������͵�ָ�룩
//����ֵ���߼���
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
//д���ڴ�
//����1������ID����������Ϊ�����ͣ�
//����2��Ҫд����ڴ��ַ����������Ϊ�������͵�ָ�룩
//����3��Ҫд����ڴ��С����������Ϊ�����ͣ�
//����4��Ҫд����ڴ����ݣ���������Ϊ�������͵�ָ�룩
//����ֵ���߼���
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
//���ñ�������ID
//����1������ID����������Ϊ�����ͣ�
//����ֵ���߼���
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