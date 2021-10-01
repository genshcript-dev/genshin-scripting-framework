#pragma once

#include "sdk/sdk.h"

namespace game
{
	inline game::sdk::get_dx_swapchain_t get_dx_swapchain = nullptr;

	inline game::sdk::player_map_coords *player_map_coords = nullptr;

	inline game::sdk::UnityEngine_Cursor_get_visible_t            UnityEngine_Cursor_get_visible            ("UnityEngine.Cursor::get_visible()");
	inline game::sdk::UnityEngine_Cursor_set_visible_t            UnityEngine_Cursor_set_visible            ("UnityEngine.Cursor::set_visible(System.Boolean)");
	inline game::sdk::UnityEngine_Cursor_get_lockState_t          UnityEngine_Cursor_get_lockState          ("UnityEngine.Cursor::get_lockState()");
	inline game::sdk::UnityEngine_Cursor_set_lockState_t          UnityEngine_Cursor_set_lockState          ("UnityEngine.Cursor::set_lockState(UnityEngine.CursorLockMode)");
	inline game::sdk::UnityEngine_JsonUtility_ToJson_t            UnityEngine_JsonUtility_ToJson            ("UnityEngine.JsonUtility::ToJson(System.Object,System.Boolean)");
	inline game::sdk::UnityEngine_JsonUtility_FromJson_t          UnityEngine_JsonUtility_FromJson          ("UnityEngine.JsonUtility::FromJson(System.String,System.Type)");
	inline game::sdk::UnityEngine_JsonUtility_FromJsonOverwrite_t UnityEngine_JsonUtility_FromJsonOverwrite ("UnityEngine.JsonUtility::FromJsonOverwrite(System.String,System.Object)");
	inline game::sdk::UnityEngine_Input_GetButton_t               UnityEngine_Input_GetButton               ("UnityEngine.Input::GetButton(System.String)");
	inline game::sdk::UnityEngine_Input_GetAxisRaw_t              UnityEngine_Input_GetAxisRaw              ("UnityEngine.Input::GetAxisRaw(System.String)");
	inline game::sdk::UnityEngine_Object_get_name_t               UnityEngine_Object_get_name               ("UnityEngine.Object::get_name()");
	inline game::sdk::UnityEngine_Object_ToString_t               UnityEngine_Object_ToString               ("UnityEngine.Object::ToString()");
	inline game::sdk::UnityEngine_Camera_get_main_t               UnityEngine_Camera_get_main               ("UnityEngine.Camera::get_main()");
	inline game::sdk::UnityEngine_Component_get_transform_t       UnityEngine_Component_get_transform       ("UnityEngine.Component::get_transform()");

	inline game::sdk::UnityEngine_Transform_INTERNAL_get_position_t UnityEngine_Transform_INTERNAL_get_position ("UnityEngine.Transform::INTERNAL_get_position(UnityEngine.Vector3&)");

	inline game::sdk::il2cpp_string_chars_t  il2cpp_string_chars  = nullptr;
	inline game::sdk::il2cpp_string_length_t il2cpp_string_length = nullptr;
	inline game::sdk::il2cpp_string_new_t    il2cpp_string_new    = nullptr;

	bool init();
}