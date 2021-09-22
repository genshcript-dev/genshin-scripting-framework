#include "tab_scripts.h"
#include <imgui.h>
#include <string>
#include "../script_manager.h"
#include "../helpers/imgui_prompts.h"
#include <misc_utils.h>

static const char         *import_prompt_error_message         = "No error message provided.";
static bool                import_prompt_error_message_visible = false;
static utils::fader_float  import_prompt_error_message_fader   = utils::fader_float(1000, 3000);

void callback_import_prompt_ui()
{
    static char buffer_import[256] = { '\0' };

    ImGui::Text("Script Path:");
    ImGui::SameLine();
    bool input_enter_keyed = ImGui::InputText("##script_import_buffer", buffer_import, sizeof(buffer_import), ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::SameLine();
    if (ImGui::Button("Import") || input_enter_keyed)
    {
        if (gsf::script_manager::script_import(buffer_import))
        {
            std::memset(buffer_import, '\0', sizeof(buffer_import));
            ImGui::CloseCurrentPopup();
        }
        else
        {
            import_prompt_error_message = "Failed to import script!";
            import_prompt_error_message_fader.mark();
            import_prompt_error_message_visible = true;
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel"))
        ImGui::CloseCurrentPopup();

    if (import_prompt_error_message_visible)
    {
        float errmsg_alpha_scale = import_prompt_error_message_fader.get();
        if (errmsg_alpha_scale > 0.f)
        {
            ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, ImVec4{ 1.f, 0.f, 0.f, errmsg_alpha_scale });
            ImGui::TextWrapped(import_prompt_error_message);
            ImGui::PopStyleColor();
        }
        else
        {
            import_prompt_error_message_visible = false;
        }
    }
}
static helpers::imgui_popup_modal import_prompt = helpers::imgui_popup_modal("Import Script", &callback_import_prompt_ui);

void gsf::menu::tab_scripts::render_tab()
{
	if (ImGui::BeginTabItem("Scripts"))
	{
        if (ImGui::BeginTable("##gsf_script_table", 3, ImGuiTableFlags_::ImGuiTableFlags_ScrollY, { 0.f , ImGui::GetWindowHeight() - ImGui::GetCursorPosY() - 35.f }))
		{
            ImGui::TableSetupColumn("Script Name");
            ImGui::TableSetupColumn("Notice");
            ImGui::TableSetupColumn("Status");
            ImGui::TableHeadersRow();

            for (const auto &script : gsf::script_manager::get_scripts())
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Selectable(script->get_filename().data(), &script->_tab_script_selected, ImGuiSelectableFlags_::ImGuiSelectableFlags_SpanAllColumns);
                ImGui::TableNextColumn();
                ImGui::Text(script->_tab_script_notice.c_str());
                ImGui::TableNextColumn();
                ImGui::Text(gsf::script::state_to_cstr(script->get_current_state()));
            }

			ImGui::EndTable();
		}

        ImGui::Separator();

        if (ImGui::Button("Load"))
        {
            for (const auto &script : gsf::script_manager::get_scripts())
            {
                if (script->_tab_script_selected && script->get_current_state() == gsf::script::state::UNLOADED)
                    script->h_thread_loading(gsf::script::state::LOAD);
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Unload"))
        {
            for (const auto &script : gsf::script_manager::get_scripts())
            {
                if (script->_tab_script_selected && script->get_current_state() == gsf::script::state::LOADED)
                    script->h_thread_loading(gsf::script::state::UNLOAD);
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Deselect All"))
        {
            for (const auto &script : gsf::script_manager::get_scripts())
                script->_tab_script_selected = false;
        }

        ImGui::SameLine();
        if (ImGui::Button("Import Script"))
        {
            import_prompt.show();
        }

		ImGui::EndTabItem();
	}
}