#pragma once

#include "cs_types.h"
#include "il2cpp.h"
#include <vector>
#include <macro.h>
#include <d3d11.h>

namespace game::sdk
{
	using get_fn_t = void *(*)(const char *);
	using get_dx_swapchain_t = IDXGISwapChain *(*)(void);

	class _unity_scripting_api
	{
	public:
		_unity_scripting_api(const char *name_)
			: name(name_)
		{
			this->_instances.emplace_back(this);
		};

		bool load_function()
		{
			return DEBUG_CON_C_LOG(this->name, (this->function = this->get_api_by_name(this->name)));
		}

		inline static bool load_function_all()
		{
			bool is_success = true;
			for (const auto &api : _unity_scripting_api::_instances)
			{
				if (!api->load_function())
				{
					is_success = false;
					break;
				}
			}

			_unity_scripting_api::_instances.clear();
			return is_success;
		}

	protected:
		void *function = nullptr;
		const char *name;

	public:
		inline static get_fn_t get_api_by_name = nullptr; // TODO: reverse this and then implement hashable version

	private:
		inline static std::vector<_unity_scripting_api *> _instances;
	};

	template <typename fn_ret_t = void, typename... fn_args_t>
	class unity_scripting_api : public _unity_scripting_api
	{
	public:

		using type = fn_ret_t(*)(fn_args_t...);

		unity_scripting_api(const char *name_)
			: _unity_scripting_api(name_)
		{};

		fn_ret_t operator()(fn_args_t... args)
		{
			return reinterpret_cast<type>(this->function)(args...);
		}

		void *get_ptr()
		{
			return this->function;
		}
	};

	enum class CursorLockMode : int
	{
		None,
		Locked,
		Confined
	};

	struct U_Vector3_t
	{
		float x, y, z;
	};

	using U_Component_t = void;
	using U_Transform_t = void;
	using U_Camera_t = void;

	using UnityEngine_Cursor_get_visible_t   = unity_scripting_api<CS_Boolean_t>;
	using UnityEngine_Cursor_set_visible_t   = unity_scripting_api<void, CS_Boolean_t>;
	using UnityEngine_Cursor_get_lockState_t = unity_scripting_api<game::sdk::CursorLockMode>;
	using UnityEngine_Cursor_set_lockState_t = unity_scripting_api<void, CursorLockMode>;

	using UnityEngine_JsonUtility_ToJson_t            = unity_scripting_api<Il2CppString *, CS_Object_t *, CS_Boolean_t>;
	using UnityEngine_JsonUtility_FromJson_t          = unity_scripting_api<CS_Object_t *, Il2CppString *, CS_Type_t *>;
	using UnityEngine_JsonUtility_FromJsonOverwrite_t = unity_scripting_api<void, Il2CppString *, CS_Object_t *>;

	using UnityEngine_Input_GetButton_t  = unity_scripting_api<CS_Boolean_t, Il2CppString *>;
	using UnityEngine_Input_GetAxisRaw_t = unity_scripting_api<float, Il2CppString *>;

	using UnityEngine_Object_get_name_t = unity_scripting_api<Il2CppString *, CS_Object_t *>;
	using UnityEngine_Object_ToString_t = unity_scripting_api<Il2CppString *, CS_Object_t *>;

	using UnityEngine_Camera_get_main_t = unity_scripting_api<U_Camera_t *>;

	using UnityEngine_Component_get_transform_t = unity_scripting_api<U_Transform_t *, U_Component_t *>;

	using UnityEngine_Transform_INTERNAL_get_position_t = unity_scripting_api<void, U_Transform_t *, U_Vector3_t *>;
}