#include "def.h"
PVOID RegistrationHandle = NULL;
HANDLE ProtectProcessId = NULL;
OB_PREOP_CALLBACK_STATUS PobPreOperationCallback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation);
NTSTATUS StartObCallBack() {
	NTSTATUS Status;
	OB_CALLBACK_REGISTRATION ObReg = { 0 };
	OB_OPERATION_REGISTRATION OpReg = { 0 };
	ObReg.Version = OB_FLT_REGISTRATION_VERSION;
	ObReg.OperationRegistrationCount = 1;
	ObReg.RegistrationContext = NULL;
	RtlInitUnicodeString(&ObReg.Altitude, L"166666");
	OpReg.ObjectType = PsProcessType;
	OpReg.Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;
	OpReg.PreOperation = PobPreOperationCallback;
	ObReg.OperationRegistration = &OpReg;
	Status = ObRegisterCallbacks(&ObReg, &RegistrationHandle);
	return Status;
}
VOID StopObCallBack() {
	if (RegistrationHandle) {
		ObUnRegisterCallbacks(RegistrationHandle);
		RegistrationHandle = NULL;
	}
}
OB_PREOP_CALLBACK_STATUS PobPreOperationCallback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION OperationInformation)
{
	UNREFERENCED_PARAMETER(RegistrationContext);
	HANDLE ProcessId = PsGetProcessId((PEPROCESS)OperationInformation->Object);
	if (ProtectProcessId == ProcessId && OperationInformation->ObjectType == *PsProcessType)
	{
		DbgPrint("Protect process is be open");
		if (OperationInformation->Operation == OB_OPERATION_HANDLE_CREATE)
		{
			if (OperationInformation->Parameters->CreateHandleInformation.DesiredAccess & PROCESS_TERMINATE)
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= (~PROCESS_TERMINATE);
			if (OperationInformation->Parameters->CreateHandleInformation.DesiredAccess & PROCESS_VM_OPERATION)
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= (~PROCESS_VM_OPERATION);
			if (OperationInformation->Parameters->CreateHandleInformation.DesiredAccess & PROCESS_VM_READ)
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= (~PROCESS_VM_READ);
			if (OperationInformation->Parameters->CreateHandleInformation.DesiredAccess & PROCESS_VM_WRITE)
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= (~PROCESS_VM_WRITE);
			if (OperationInformation->Parameters->CreateHandleInformation.DesiredAccess & PROCESS_SUSPEND_RESUME)
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= (~PROCESS_SUSPEND_RESUME);
		}
		if (OperationInformation->Operation == OB_OPERATION_HANDLE_DUPLICATE)
		{
			if (OperationInformation->Parameters->CreateHandleInformation.DesiredAccess & PROCESS_TERMINATE)
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= (~PROCESS_TERMINATE);
			if (OperationInformation->Parameters->CreateHandleInformation.DesiredAccess & PROCESS_VM_OPERATION)
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= (~PROCESS_VM_OPERATION);
			if (OperationInformation->Parameters->CreateHandleInformation.DesiredAccess & PROCESS_VM_READ)
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= (~PROCESS_VM_READ);
			if (OperationInformation->Parameters->CreateHandleInformation.DesiredAccess & PROCESS_VM_WRITE)
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= (~PROCESS_VM_WRITE);
			if (OperationInformation->Parameters->CreateHandleInformation.DesiredAccess & PROCESS_SUSPEND_RESUME)
				OperationInformation->Parameters->CreateHandleInformation.DesiredAccess &= (~PROCESS_SUSPEND_RESUME);
		}
	}
	return OB_PREOP_SUCCESS;
}
NTSTATUS SetProtectProcessId(HANDLE ProcessId) {
	ProtectProcessId = ProcessId;
	DbgPrint("Protect process id = %d", ProcessId);
	return STATUS_SUCCESS;
}