//#include "includes.h"
//using namespace hazedumper;
//
//struct gameOffsets
//{
//	DWORD IPlayer = signatures::dwLocalPlayer;
//	DWORD fJump = signatures::dwForceJump;
//	DWORD flags = netvars::m_fFlags;
//}offsets;
//
//struct values
//{
//	DWORD localPlayer;
//	DWORD gameModule;
//	int flag;
//}val;
//
//int main()
//{
//	AllocConsole();
//	freopen("CONOUT$", "w", stdout);
//
//	val.gameModule = (DWORD)GetModuleHandle("client_panorama.dll");
//	val.localPlayer = *(DWORD*)(val.gameModule + offsets.IPlayer);
//
//	if (val.localPlayer == NULL)
//		while (val.localPlayer == NULL)
//			val.localPlayer = *(DWORD*)(val.gameModule + offsets.IPlayer);
//
//	std::cout << std::hex << val.localPlayer << std::endl;
//
//	bool BunnyOn = false;
//
//	while (!GetAsyncKeyState(VK_INSERT) & 1)
//	{
//		while (GetAsyncKeyState(VK_F1) & 1)
//		{
//			BunnyOn = !BunnyOn;
//			std::cout << BunnyOn << std::endl;
//			Sleep(5);
//		}
//
//		if (BunnyOn && (GetAsyncKeyState(VK_SPACE))) Bhop();
//
//		Sleep(5);
//	}
//	return 0;
//}
//
//BOOL WINAPI DllMain(   HMODULE hModule,
//                       DWORD  dwReason,
//                       LPVOID lpReserved
//                    )
//{
//	switch (dwReason)
//	{
//	case DLL_PROCESS_ATTACH:
//		DisableThreadLibraryCalls(hModule);
//		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
//		break;
//	default:
//		break;
//	}
//
//	return TRUE;
//}
//
