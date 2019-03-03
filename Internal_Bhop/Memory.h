#pragma once

#include <string_view>

class Memory final {
public: 
	Memory() noexcept;
	std::uintptr_t findPattern(const std::string&, const std::string&) const;

	std::uintptr_t present;
	std::uintptr_t reset;

private:
};

extern Memory memory;