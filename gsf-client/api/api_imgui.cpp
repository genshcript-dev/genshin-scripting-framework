#include "api_imgui.h"
#include <imgui.h>

bool gsf::api_imgui::setup_api(sol::state &slua)
{
	auto namespace_imgui = slua["imgui"].get_or_create<sol::table>();
	namespace_imgui.set_function("begin", &gsf::api_imgui::_api_begin, this);
	namespace_imgui.set_function("iend", &gsf::api_imgui::_api_iend, this);
	namespace_imgui.set_function("text", [](const char *text) { ImGui::Text(text); });
	namespace_imgui.set_function("same_line", []() { ImGui::SameLine(); });
	namespace_imgui.set_function("button", [](const char *text) -> bool { return ImGui::Button(text); });
	namespace_imgui.set_function("separator", &ImGui::Separator);
	namespace_imgui.set_function("push_id", static_cast<void(*)(int)>(&ImGui::PushID));
	namespace_imgui.set_function("pop_id", &ImGui::PopID);

	return true;
}

bool gsf::api_imgui::_api_begin(const char *text)
{
	++this->imgui_active_begin_count;
	return ImGui::Begin(text, nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);
}

void gsf::api_imgui::_api_iend()
{
	--this->imgui_active_begin_count;
	return ImGui::End();
}
