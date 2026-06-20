#include <ntifs.h>
#include "common.hpp"

typedef enum _SYSTEM_INFORMATION_CLASS {
    SystemProcessInformation = 5
} SYSTEM_INFORMATION_CLASS;

typedef struct _SYSTEM_PROCESS_INFORMATION {
    ULONG           NextEntryOffset;
    ULONG           NumberOfThreads;
    LARGE_INTEGER   Reserved[3];
    LARGE_INTEGER   CreateTime;
    LARGE_INTEGER   UserTime;
    LARGE_INTEGER   KernelTime;
    UNICODE_STRING  ImageName;
    KPRIORITY       BasePriority;
    HANDLE          UniqueProcessId;
    HANDLE          InheritedFromUniqueProcessId;
    ULONG           HandleCount;
    ULONG           Reserved2[2];
    ULONG           PrivatePageCount;
    VM_COUNTERS     VirtualMemoryCounters;
    IO_COUNTERS     IoCounters;
} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;

extern "C" {
    NTSTATUS NTAPI ZwQuerySystemInformation(
        SYSTEM_INFORMATION_CLASS SystemInformationClass,
        PVOID SystemInformation,
        ULONG SystemInformationLength,
        PULONG ReturnLength);

    NTKERNELAPI NTSTATUS IoCreateDriver(
        PUNICODE_STRING DriverName,
        PDRIVER_INITIALIZE InitializationFunction);

    NTKERNELAPI NTSTATUS MmCopyVirtualMemory(
        PEPROCESS SourceProcess,
        PVOID SourceAddress,
        PEPROCESS TargetProcess,
        PVOID TargetAddress,
        SIZE_T BufferSize,
        KPROCESSOR_MODE PreviousMode,
        PSIZE_T ReturnSize);

    NTKERNELAPI PVOID PsGetProcessSectionBaseAddress(PEPROCESS Process);
    NTKERNELAPI PPEB  PsGetProcessPeb(PEPROCESS Process);
}

namespace driver
{
    static PEPROCESS   g_targetProcess{ nullptr };
    static PDEVICE_OBJECT g_deviceObject{ nullptr };
    static UNICODE_STRING g_dosName{};

    NTSTATUS create(PDEVICE_OBJECT device_object, PIRP irp)
    {
        UNREFERENCED_PARAMETER(device_object);
        irp->IoStatus.Status = STATUS_SUCCESS;
        irp->IoStatus.Information = 0;
        IoCompleteRequest(irp, IO_NO_INCREMENT);
        return STATUS_SUCCESS;
    }

    NTSTATUS close(PDEVICE_OBJECT device_object, PIRP irp)
    {
        UNREFERENCED_PARAMETER(device_object);
        irp->IoStatus.Status = STATUS_SUCCESS;
        irp->IoStatus.Information = 0;
        IoCompleteRequest(irp, IO_NO_INCREMENT);
        return STATUS_SUCCESS;
    }

    NTSTATUS deviceControl(PDEVICE_OBJECT device_object, PIRP irp)
    {
        UNREFERENCED_PARAMETER(device_object);

        const auto stack = IoGetCurrentIrpStackLocation(irp);
        if (!stack)
        {
            irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
            irp->IoStatus.Information = 0;
            IoCompleteRequest(irp, IO_NO_INCREMENT);
            return STATUS_UNSUCCESSFUL;
        }

        const ULONG code = stack->Parameters.DeviceIoControl.IoControlCode;
        PVOID sysBuf = irp->AssociatedIrp.SystemBuffer;
        NTSTATUS status = STATUS_UNSUCCESSFUL;
        SIZE_T bytesIO = 0;

        if (code == IOCTL_ATTACH)
        {
            // process_id ile hedef processe baglan
            const auto req = static_cast<Request*>(sysBuf);
            if (!req || !req->process_id) { status = STATUS_INVALID_PARAMETER; goto done; }

            if (g_targetProcess)
            {
                ObDereferenceObject(g_targetProcess);
                g_targetProcess = nullptr;
            }

            status = PsLookupProcessByProcessId(req->process_id, &g_targetProcess);
            bytesIO = sizeof(Request);
        }
        else if (code == IOCTL_READ)
        {
            const auto req = static_cast<Request*>(sysBuf);
            if (!req || !req->target || !req->buffer || req->size == 0 || !g_targetProcess)
            {
                status = STATUS_INVALID_PARAMETER;
                goto done;
            }

            SIZE_T copied = 0;
            PEPROCESS callerProcess = IoGetRequestorProcess(irp);
            if (!callerProcess) callerProcess = PsGetCurrentProcess();
            status = MmCopyVirtualMemory(
                g_targetProcess,
                req->target,
                callerProcess,
                req->buffer,
                req->size,
                UserMode,
                &copied);

            bytesIO = sizeof(Request);
        }
        else if (code == IOCTL_GET_PID)
        {
            // MemoryMgr::GetProcessID -> PID_PACK
            const auto req = static_cast<PID_PACK*>(sysBuf);
            if (!req) { status = STATUS_INVALID_PARAMETER; goto done; }

            // process listesini tara, isim eslesmesini bul
            ULONG retLen = 0;
            ZwQuerySystemInformation(SystemProcessInformation, nullptr, 0, &retLen);
            if (retLen == 0) { status = STATUS_UNSUCCESSFUL; goto done; }

            PVOID buf = ExAllocatePool2(POOL_FLAG_NON_PAGED, retLen + 0x1000, 'C2CP');
            if (!buf) { status = STATUS_INSUFFICIENT_RESOURCES; goto done; }

            status = ZwQuerySystemInformation(SystemProcessInformation, buf, retLen + 0x1000, &retLen);
            if (!NT_SUCCESS(status)) { ExFreePool(buf); goto done; }

            UNICODE_STRING targetName{};
            RtlInitUnicodeString(&targetName, req->name);

            auto entry = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION>(buf);
            req->pid = 0;
            while (true)
            {
                if (entry->ImageName.Buffer &&
                    RtlEqualUnicodeString(&entry->ImageName, &targetName, TRUE))
                {
                    req->pid = static_cast<UINT32>(
                        reinterpret_cast<ULONG_PTR>(entry->UniqueProcessId));
                    break;
                }
                if (entry->NextEntryOffset == 0) break;
                entry = reinterpret_cast<PSYSTEM_PROCESS_INFORMATION>(
                    reinterpret_cast<PUCHAR>(entry) + entry->NextEntryOffset);
            }
            ExFreePool(buf);

            status = (req->pid != 0) ? STATUS_SUCCESS : STATUS_NOT_FOUND;
            bytesIO = sizeof(PID_PACK);
        }
        else if (code == IOCTL_GET_MODULE_BASE)
        {
            const auto req = static_cast<MODULE_PACK*>(sysBuf);
            if (!req || req->pid == 0) { status = STATUS_INVALID_PARAMETER; goto done; }

            PEPROCESS process = nullptr;
            status = PsLookupProcessByProcessId(ULongToHandle(req->pid), &process);
            if (!NT_SUCCESS(status)) goto done;

            req->baseAddress = 0;
            req->size = 0;

            // moduleName bos ise ana EXE base'ini don
            if (req->moduleName[0] == L'\0')
            {
                req->baseAddress = reinterpret_cast<UINT64>(PsGetProcessSectionBaseAddress(process));
            }
            else
            {
                // PEB uzerinden DLL listesini tara
                KAPC_STATE apcState{};
                KeStackAttachProcess(process, &apcState);

                __try
                {
                    PPEB peb = PsGetProcessPeb(process);
                    if (peb && peb->Ldr)
                    {
                        PLIST_ENTRY head = &peb->Ldr->InMemoryOrderModuleList;
                        PLIST_ENTRY cur = head->Flink;
                        UNICODE_STRING targetMod{};
                        RtlInitUnicodeString(&targetMod, req->moduleName);

                        while (cur != head)
                        {
                            auto entry = CONTAINING_RECORD(cur, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
                            if (RtlEqualUnicodeString(&entry->BaseDllName, &targetMod, TRUE))
                            {
                                req->baseAddress = reinterpret_cast<UINT64>(entry->DllBase);
                                req->size = entry->SizeOfImage;
                                break;
                            }
                            cur = cur->Flink;
                        }
                    }
                }
                __except (EXCEPTION_EXECUTE_HANDLER) {}

                KeUnstackDetachProcess(&apcState);
            }

            ObDereferenceObject(process);
            status = (req->baseAddress != 0) ? STATUS_SUCCESS : STATUS_NOT_FOUND;
            bytesIO = sizeof(MODULE_PACK);
        }
        else if (code == IOCTL_BATCH_READ)
        {
            // MemoryMgr::BatchReadMemory -> BatchReadHeader + BatchReadRequest[] + output buf
            const auto header = static_cast<BatchReadHeader*>(sysBuf);
            if (!header || !header->process_id || header->num_requests == 0 || !g_targetProcess)
            {
                status = STATUS_INVALID_PARAMETER;
                goto done;
            }

            auto requests = reinterpret_cast<BatchReadRequest*>(
                reinterpret_cast<PUCHAR>(header) + sizeof(BatchReadHeader));
            auto outBuf = reinterpret_cast<PUCHAR>(header)
                + sizeof(BatchReadHeader)
                + header->num_requests * sizeof(BatchReadRequest);

            for (UINT32 i = 0; i < header->num_requests; i++)
            {
                SIZE_T copied = 0;
                MmCopyVirtualMemory(
                    g_targetProcess,
                    reinterpret_cast<PVOID>(requests[i].address),
                    PsGetCurrentProcess(),
                    outBuf + requests[i].offset_in_buffer,
                    requests[i].size,
                    KernelMode,
                    &copied);
            }

            status = STATUS_SUCCESS;
            bytesIO = sizeof(BatchReadHeader)
                + header->num_requests * sizeof(BatchReadRequest)
                + header->total_buffer_size;
        }

    done:
        irp->IoStatus.Status = status;
        irp->IoStatus.Information = bytesIO;
        IoCompleteRequest(irp, IO_NO_INCREMENT);
        return status;
    }

    NTSTATUS unload(PDRIVER_OBJECT driver_object)
    {
        UNREFERENCED_PARAMETER(driver_object);
        if (g_targetProcess)
        {
            ObDereferenceObject(g_targetProcess);
            g_targetProcess = nullptr;
        }
        IoDeleteSymbolicLink(&g_dosName);
        if (g_deviceObject)
            IoDeleteDevice(g_deviceObject);
        return STATUS_SUCCESS;
    }
}

NTSTATUS driverMain(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path)
{
    UNREFERENCED_PARAMETER(registry_path);

    UNICODE_STRING device_name{};
    RtlInitUnicodeString(&device_name, C2C_DEVICE_NAME);
    RtlInitUnicodeString(&driver::g_dosName, C2C_DOS_NAME);

    NTSTATUS status = IoCreateDevice(
        driver_object,
        0,
        &device_name,
        FILE_DEVICE_UNKNOWN,
        FILE_DEVICE_SECURE_OPEN,
        FALSE,
        &driver::g_deviceObject);

    if (!NT_SUCCESS(status)) return status;

    status = IoCreateSymbolicLink(&driver::g_dosName, &device_name);
    if (!NT_SUCCESS(status))
    {
        IoDeleteDevice(driver::g_deviceObject);
        return status;
    }

    SetFlag(driver::g_deviceObject->Flags, DO_BUFFERED_IO);

    driver_object->MajorFunction[IRP_MJ_CREATE]         = driver::create;
    driver_object->MajorFunction[IRP_MJ_CLOSE]          = driver::close;
    driver_object->MajorFunction[IRP_MJ_DEVICE_CONTROL] = driver::deviceControl;
    driver_object->DriverUnload = reinterpret_cast<PDRIVER_UNLOAD>(driver::unload);

    ClearFlag(driver::g_deviceObject->Flags, DO_DEVICE_INITIALIZING);

    return STATUS_SUCCESS;
}

NTSTATUS DriverEntry()
{
    UNICODE_STRING driverName{};
    RtlInitUnicodeString(&driverName, C2C_DRIVER_NAME);
    return IoCreateDriver(&driverName, &driverMain);
}
