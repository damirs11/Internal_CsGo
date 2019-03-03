#include <array>
#include <sstream>
#include <Windows.h>

#include "imgui/imgui.h"
#include "GUI.h"
#include "Config.h"

constexpr auto windowFlags = ImGuiWindowFlags_NoCollapse 
| ImGuiWindowFlags_NoResize 
| ImGuiWindowFlags_NoScrollbar 
| ImGuiWindowFlags_NoScrollWithMouse;

void GUI::render() noexcept
{
	renderMenuBar();
	renderMiscWindow();
}

void GUI::checkBoxAndColorPicker(const std::string_view name, bool* enable, float* color) noexcept
{
	ImGui::Checkbox(std::string{ "##" }.append(name).c_str(), enable);
	ImGui::SameLine(0.0f, 5.0f);
	bool openPopup = ImGui::ColorButton(std::string{ "##" }.append(name).append("ColorButton").c_str(), ImColor{ color[0], color[1], color[2] }, ImGuiColorEditFlags_NoTooltip);
	ImGui::SameLine(0.0f, 5.0f);
	ImGui::Text(name.data());
	if (openPopup)
		ImGui::OpenPopup(std::string{ "##" }.append(name).append("Popup").c_str());
	if (ImGui::BeginPopup(std::string{ "##" }.append(name).append("Popup").c_str())) {
		ImGui::ColorPicker3(std::string{ "##" }.append(name).append("Popup").c_str(), color, ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_NoSidePreview);
		ImGui::EndPopup();
	}
}

void GUI::hotkey(int& key) noexcept
{
	ImGui::Text((std::ostringstream{ } << "[ " << (key ? std::to_string(key) : "key") << " ]").str().c_str());
	if (ImGui::IsItemHovered()) {
		ImGuiIO& io = ImGui::GetIO();
		for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
			if (ImGui::IsKeyPressed(i) && i != VK_INSERT)
				key = i != VK_INSERT ? i : 0;
	}
}

void GUI::renderMenuBar() noexcept
{
	if (ImGui::BeginMainMenuBar()) {
		ImGui::MenuItem("Misc", nullptr, &window.misc);

		if (ImGui::BeginMenu("Settings")) {
			if (ImGui::MenuItem("Load"))
				config.load();
			if (ImGui::MenuItem("Save"))
				config.save();
			if (ImGui::MenuItem("Reset"))
				config.reset();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void GUI::renderMiscWindow() noexcept
{
	if (window.misc) {
		ImGui::SetNextWindowSize({ 220.0f, 260.0f });
		ImGui::Begin("Misc", &window.misc, windowFlags);
		ImGui::Checkbox("BunnyHop", &config.misc.bunnyHop);
		static std::array<char, 16> buffer;
		ImGui::PushItemWidth(120.0f);
		ImGui::InputText("##1", buffer.data(), buffer.size());
		ImGui::SameLine();
	}
}