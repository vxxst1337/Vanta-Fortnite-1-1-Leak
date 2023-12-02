#pragma once
#include <Windows.h>
#include <cstdint>
#include <TlHelp32.h>

#define code_read_physical CTL_CODE(FILE_DEVICE_UNKNOWN, 0x316, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_read_virtual CTL_CODE(FILE_DEVICE_UNKNOWN, 0x901, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_write CTL_CODE(FILE_DEVICE_UNKNOWN, 0x461, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_get_base_address CTL_CODE(FILE_DEVICE_UNKNOWN, 0x135, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_decrypt_cr3 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x136, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

typedef struct _rw {
	INT32 process_id;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
} rw, * WriteStruct;


typedef struct _r {
	INT32 process_id;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
} rwr, * ReadStruct;


typedef struct _ba {
	INT32 process_id;
	ULONGLONG* address;
} ba, * BaseAddressStruct;

typedef struct _cr3 {
	INT32 process_id;
} cr3, * DTBStruct;


namespace mem {
	HANDLE driver_handle;
	INT32 process_id;

	bool VirtualMode;

	inline bool SetupDriver();

	inline void read(PVOID address, PVOID buffer, DWORD size);

	inline void write_physical(PVOID address, PVOID buffer, DWORD size);

	inline uintptr_t get_base_address();

	inline uintptr_t Attach(LPCTSTR process_name);

	inline void bShouldUseVirtualMode(bool mode);

	inline void GetDTB();
}

class Kern
{
public:
	template <typename T>
	inline T read(uint64_t address) {
		T buffer{ };
		mem::read((PVOID)address, &buffer, sizeof(T));
		return buffer;
	}

	template <typename T>
	inline T write(uint64_t address, T buffer) {

		mem::write_physical((PVOID)address, &buffer, sizeof(T));
		return buffer;
	}
}; Kern* request;

inline void mem::bShouldUseVirtualMode(bool mode)
{
	VirtualMode = mode;
}

inline uintptr_t mem::get_base_address() {
	uintptr_t image_address = { NULL };
	_ba arguments = { NULL };

	arguments.process_id = process_id;
	arguments.address = (ULONGLONG*)&image_address;

	DeviceIoControl(driver_handle, code_get_base_address, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

	return image_address;
}

inline void mem::write_physical(PVOID address, PVOID buffer, DWORD size) {
	_rw arguments = { 0 };

	arguments.address = (ULONGLONG)address;
	arguments.buffer = (ULONGLONG)buffer;
	arguments.size = size;
	arguments.process_id = process_id;

	DeviceIoControl(driver_handle, code_write, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
}

inline void mem::read(PVOID address, PVOID buffer, DWORD size) {
	_r arguments = { 0 };
	arguments.address = (ULONGLONG)address;
	arguments.buffer = (ULONGLONG)buffer;
	arguments.size = size;
	arguments.process_id = process_id;


	if (VirtualMode)
	{
		DeviceIoControl(driver_handle, code_read_virtual, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	}
	else {
		DeviceIoControl(driver_handle, code_read_physical, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	}
}

inline uintptr_t mem::Attach(LPCTSTR process_name) {
	PROCESSENTRY32 pt;
	HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pt.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hsnap, &pt)) {
		do {
			if (!lstrcmpi(pt.szExeFile, process_name)) {
				CloseHandle(hsnap);
				process_id = pt.th32ProcessID;
				return pt.th32ProcessID;
			}
		} while (Process32Next(hsnap, &pt));
	}
	CloseHandle(hsnap);

	return { NULL };
}

inline bool mem::SetupDriver() {
	driver_handle = CreateFileA(("\\\\.\\\hoqlisdetected"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (!driver_handle || (driver_handle == INVALID_HANDLE_VALUE))
		return false;

	return true;
}

inline void mem::GetDTB()
{
	_cr3 arguments = { NULL };

	arguments.process_id = process_id;

	DeviceIoControl(driver_handle, code_decrypt_cr3, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
}