#pragma once

#include "sdk/sdk.h"

namespace game
{
	inline game::sdk::player_map_coords *player_map_coords = nullptr;
	inline game::sdk::get_fn_t get_fn = nullptr;
	inline game::sdk::UnityEngine_Cursor_get_visible_t engine_cursor_get_visible = nullptr;
	inline game::sdk::UnityEngine_Cursor_set_visible_t engine_cursor_set_visible = nullptr;
	inline game::sdk::UnityEngine_Cursor_get_lockState_t engine_cursor_get_lockstate = nullptr;
	inline game::sdk::UnityEngine_Cursor_set_lockState_t engine_cursor_set_lockstate = nullptr;

	bool init();
}