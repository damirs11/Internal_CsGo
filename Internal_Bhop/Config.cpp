#include <fstream>
#include <ShlObj.h>

#include "cereal/archives/binary.hpp"

#include "Config.h"

Config::Config(const std::string_view name) noexcept
{
	PWSTR pathToDocuments;
	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &pathToDocuments)))
	{
		path = pathToDocuments;
		path /= name;
		CoTaskMemFree(pathToDocuments);
	}
}

void Config::load() noexcept
{
	std::ifstream in{ path };

	if (!in.good())
		return;

	cereal::BinaryInputArchive inputArchive{ in };
	inputArchive(misc);
	in.close();
}

void Config::save() const noexcept
{
	std::ofstream out{ path };

	if (!out.good())
		return;

	cereal::BinaryOutputArchive outputArchive{ out };
	outputArchive(misc);
	out.close();
}

void Config::reset() noexcept
{
	misc = { };
}
