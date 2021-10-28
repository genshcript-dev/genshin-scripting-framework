#pragma once

#include "unity_object.h"
#include "unity_vector3.h"

namespace game::sdk
{
	class Transform : public game::sdk::Object
	{
	public:
		void get_position(game::sdk::Vector3 &position_out)
		{
			this->_INTERNAL_get_position(this, &position_out);
		}

		game::sdk::Vector3 get_position()
		{
			Vector3 out;
			this->_INTERNAL_get_position(this, &out);
			return out;
		}

	public:
		inline static game::sdk::unity_scripting_api<void, game::sdk::Transform *, game::sdk::Vector3 *> _INTERNAL_get_position = ("UnityEngine.Transform::INTERNAL_get_position(UnityEngine.Vector3&)");
	};
}

GSF_UNITY_SDK_ENSURE_NO_NONSTATIC(game::sdk::Transform);