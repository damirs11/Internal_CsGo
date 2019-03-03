#include <regex>
#include <stdexcept>
#include <Windows.h>
#include <Psapi.h>

#include "Interfaces.h"
#include "Memory.h"

Memory::Memory() noexcept
{
	try {
		present = findPattern("gameoverlayrenderer", "\xFF\x15.{4}\x8B\xF8\x85\xDB") + 2;
		reset = findPattern("gameoverlayrenderer", "\xC7\x45.{5}\xFF\x15.{4}\x8B\xF8") + 9;
	}
	catch (const std::runtime_error& e) {
		MessageBox(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
		std::exit(EXIT_FAILURE);
	}
}

uintptr_t Memory::findPattern(const std::string& module, const std::string& pattern) const
{
	MODULEINFO moduleInfo;

	GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(module.c_str()), &moduleInfo, sizeof(moduleInfo));
	
	const char* begin = reinterpret_cast<const char*>(moduleInfo.lpBaseOfDll);
	const char* end = begin + moduleInfo.SizeOfImage;

	std::cmatch match;

	int searchCount = 0;
	while (!std::regex_search(begin, end, match, std::regex{ pattern, std::regex::optimize }) && searchCount < 5)
		searchCount++;

	if (searchCount == 5)
		throw std::runtime_error{ "Memory pattern scan in " + module + " failed after 5 tries!" };
	else
		return reinterpret_cast<ptrdiff_t>(moduleInfo.lpBaseOfDll) + match.position();
}

