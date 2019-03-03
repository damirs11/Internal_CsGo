#pragma once

#include <filesystem>
#include <string_view>

class Config final {
public:
	explicit Config(const std::string_view) noexcept;
	void load() noexcept;
	void save() const noexcept;
	void reset() noexcept;

	struct {
		bool bunnyHop{ false };
		bool animatedClanTag{ false };
		bool fastDuck{ false };
		bool autoPistol{ false };
		bool radarHack{ false };
		bool spectatorList{ false };
		bool watermark{ false };

		template <class Archive>
		constexpr void serialize(Archive& archive) noexcept
		{
			archive(bunnyHop, animatedClanTag, fastDuck,
				autoPistol, radarHack, spectatorList, watermark);
		}
	} misc;

private:
	std::filesystem::path path;
};

extern Config config;