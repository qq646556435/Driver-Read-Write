#include "def.h"
NTSTATUS GetModuleBase(HANDLE ProcessId, UNICODE_STRING ModuleName, PVOID Buffer) {
	NTSTATUS Status = STATUS_SUCCESS;
	PEPROCESS Process = NULL;
	KAPC_STATE ApcState = { 0 };
	BOOLEAN IsWow64 = FALSE;
	ULONG64 MouduleAddress = 0;
	UNICODE_STRING CurrentName = { 0 };
	Status = PsLookupProcessByProcessId(ProcessId, &Process);
	if (!Process) {
		Status = STATUS_UNSUCCESSFUL;
		return Status;
	}
	IsWow64 = (PsGetProcessWow64Process(Process) != NULL) ? TRUE : FALSE;
	ObDereferenceObject(Process);
	KeStackAttachProcess(Process, &ApcState);
	if (IsWow64)
	{
		PPEB32 Peb = (PPEB32)PsGetProcessWow64Process(Process);
		for (PLIST_ENTRY32 ListEntry = (PLIST_ENTRY32)((PPEB_LDR_DATA32)Peb->Ldr)->InLoadOrderModuleList.Flink;
			ListEntry != &((PPEB_LDR_DATA32)Peb->Ldr)->InLoadOrderModuleList;
			ListEntry = (PLIST_ENTRY32)ListEntry->Flink)
		{
			PLDR_DATA_TABLE_ENTRY32 LdrEntry = CONTAINING_RECORD(ListEntry, LDR_DATA_TABLE_ENTRY32, InLoadOrderLinks);
			if (LdrEntry->BaseDllName.Buffer == 0) continue;
			RtlInitUnicodeString(&CurrentName, (PWCHAR)LdrEntry->BaseDllName.Buffer);
			if (RtlEqualUnicodeString(&ModuleName, &CurrentName, TRUE))
			{
				MouduleAddress = (ULONG64)LdrEntry->DllBase;
				break;
			}
		}
	}
	else
	{
		PPEB64 pPeb = (PPEB64)PsGetProcessPeb(Process);
		for (PLIST_ENTRY64 ListEntry = (PLIST_ENTRY64)((PPEB_LDR_DATA64)pPeb->Ldr)->InLoadOrderModuleList.Flink;
			ListEntry != &((PPEB_LDR_DATA64)pPeb->Ldr)->InLoadOrderModuleList;
			ListEntry = (PLIST_ENTRY64)ListEntry->Flink)
		{
			PLDR_DATA_TABLE_ENTRY64 LdrEntry = CONTAINING_RECORD(ListEntry, LDR_DATA_TABLE_ENTRY64, InLoadOrderLinks);
			if (LdrEntry->BaseDllName.Buffer == 0) continue;
			RtlInitUnicodeString(&CurrentName, (PWCHAR)LdrEntry->BaseDllName.Buffer);
			if (RtlEqualUnicodeString(&CurrentName, &ModuleName, TRUE))
			{
				MouduleAddress = LdrEntry->DllBase;
				break;
			}
		}
	}
	RtlCopyMemory(Buffer, &MouduleAddress, sizeof(ULONG64));
	KeUnstackDetachProcess(&ApcState);
	return Status;
}
NTSTATUS ReadMemory(HANDLE ProcessId, PVOID Address, ULONG Size, PVOID Buffer) {
	NTSTATUS Status = STATUS_SUCCESS;
	PEPROCESS Process = NULL;
	KAPC_STATE ApcState = { 0 };
	PMDL Mdl = NULL;
	PVOID NewMemory = NULL;
	Status = PsLookupProcessByProcessId(ProcessId, &Process);
	if (!Process) {
		Status = STATUS_UNSUCCESSFUL;
		return Status;
	}
	ObDereferenceObject(Process);
	KeStackAttachProcess(Process, &ApcState);
	Mdl = IoAllocateMdl(Address, Size, FALSE, FALSE, NULL);
	if (!Mdl) {
		KeUnstackDetachProcess(&ApcState);
		Status = STATUS_UNSUCCESSFUL;
		return Status;
	}
	__try {
		MmProbeAndLockPages(Mdl, KernelMode, IoReadAccess);
	}
	__except (1) {
		KeUnstackDetachProcess(&ApcState);
		IoFreeMdl(Mdl);
		Status = STATUS_UNSUCCESSFUL;
		return Status;
	}
	KeUnstackDetachProcess(&ApcState);
	NewMemory = MmMapLockedPagesSpecifyCache(Mdl, KernelMode, MmCached, NULL, FALSE, HighPagePriority);
	if (!NewMemory) {
		MmUnlockPages(Mdl);
		IoFreeMdl(Mdl);
		Status = STATUS_UNSUCCESSFUL;
		return Status;
	}
	RtlCopyMemory(Buffer, NewMemory, Size);
	MmUnmapLockedPages(NewMemory, Mdl);
	MmUnlockPages(Mdl);
	IoFreeMdl(Mdl);
	return Status;
}
NTSTATUS WriteMemory(HANDLE ProcessId, PVOID Address, ULONG Size, PVOID Buffer) {
	NTSTATUS Status = STATUS_SUCCESS;
	PEPROCESS Process = NULL;
	KAPC_STATE ApcState = { 0 };
	PMDL Mdl = NULL;
	PVOID NewMemory = NULL;
	Status = PsLookupProcessByProcessId(ProcessId, &Process);
	if (!Process) {
		Status = STATUS_UNSUCCESSFUL;
		return Status;
	}
	ObDereferenceObject(Process);
	KeStackAttachProcess(Process, &ApcState);
	Mdl = IoAllocateMdl(Address, Size, FALSE, FALSE, NULL);
	if (!Mdl) {
		KeUnstackDetachProcess(&ApcState);
		Status = STATUS_UNSUCCESSFUL;
		return Status;
	}
	__try {
		MmProbeAndLockPages(Mdl, KernelMode, IoReadAccess);
	}
	__except (1) {
		KeUnstackDetachProcess(&ApcState);
		IoFreeMdl(Mdl);
		Status = STATUS_UNSUCCESSFUL;
		return Status;
	}
	KeUnstackDetachProcess(&ApcState);
	NewMemory = MmMapLockedPagesSpecifyCache(Mdl, KernelMode, MmCached, NULL, FALSE, HighPagePriority);
	if (!NewMemory) {
		MmUnlockPages(Mdl);
		IoFreeMdl(Mdl);
		Status = STATUS_UNSUCCESSFUL;
		return Status;
	}
	RtlCopyMemory(NewMemory, Buffer, Size);
	MmUnmapLockedPages(NewMemory, Mdl);
	MmUnlockPages(Mdl);
	IoFreeMdl(Mdl);
	return Status;
}