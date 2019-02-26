#include <windows.h>
#include <TlHelp32.h>
#include <iostream>
#include "offsets.h"

DWORD getIdProc(const wchar_t* procName)
{
	DWORD procId = 0;
	HANDLE hSpan = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSpan != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSpan, &procEntry))
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procName))	
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSpan, &procEntry));
		}
	}
	CloseHandle(hSpan);
	return procId;

}

uintptr_t GetBaseAddressModule(DWORD procId, const wchar_t* nameModule)
{
	uintptr_t modBaseAddress = 0;
	HANDLE hSpan = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

	if (hSpan != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		
		if (Module32First(hSpan, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, nameModule))
				{
					modBaseAddress = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSpan, &modEntry));
		}
	}
	CloseHandle(hSpan);
	return modBaseAddress;
}

uintptr_t LocalOffset = hazedumper::signatures::dwLocalPlayer;
uintptr_t forceJumpOffset = hazedumper::signatures::dwForceJump;
uintptr_t flagsOffset = hazedumper::netvars::m_fFlags;
uintptr_t OnGround = 257;

int GoUp = 5;
int GoDown = 4;
bool BunnyOn = false;


int main()
{
	DWORD procId = getIdProc(L"csgo.exe");
	HANDLE procHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);

	uintptr_t dllBaseAddress = 0;
	dllBaseAddress = GetBaseAddressModule(procId, L"client_panorama.dll");
	
	if (procHandle != 0)
	{
		std::cout << "ProcId: " << "0x" << std::hex << procId;
		std::cout << "\nAccess granted\n";
		std::cout << "dllBaseAddress: " << "0x" << std::hex << dllBaseAddress;
	}
	else
	{
		std::cout << "Access error";
		Sleep(5000);
		exit(1);
	}

	DWORD CLocalPlayer;
	int m_fFlag = 0;

	ReadProcessMemory(procHandle, (PBYTE*)(dllBaseAddress + LocalOffset), &CLocalPlayer, sizeof(DWORD), NULL);
	std::cout << "\nCLocalPlayer: " << "0x" << std::hex << CLocalPlayer;

	while (!GetAsyncKeyState(VK_F6) & 1)
	{
		if (GetAsyncKeyState(VK_F1) & 1)
		{
			BunnyOn = !BunnyOn;
			std::cout << "\nBhop: " << BunnyOn;
		}

		if (BunnyOn && (GetAsyncKeyState(VK_SPACE)))
		{
			ReadProcessMemory(procHandle, (BYTE*)(CLocalPlayer + flagsOffset), &m_fFlag, sizeof(int), NULL);
			
			if(m_fFlag == 256)
			{
				WriteProcessMemory(procHandle, (BYTE*)(dllBaseAddress + forceJumpOffset), &GoDown, sizeof(int), NULL);
			}
			else
			{
				WriteProcessMemory(procHandle, (BYTE*)(dllBaseAddress + forceJumpOffset), &GoUp, sizeof(int), NULL);
			}
		}
	}

	return 0;
}