#include "api_gsf.h"

void gsf::api_gsf::callback::reg(sol::function &function_)
{
	this->callback_function = function_;
	this->active = true;
};

void gsf::api_gsf::callback::unreg()
{
	this->callback_function = sol::nil;
	this->active = false;
};

bool gsf::api_gsf::setup_api()
{
	auto namespace_gsf = this->get_lua_state()["gsf"].get_or_create<sol::table>();
	namespace_gsf.set_function("log", &api_gsf::_api_log, this);
	namespace_gsf.set_function("register_callback", &api_gsf::_api_register_callback, this);

	return true;
}

const std::vector<std::string> &gsf::api_gsf::get_logs() const
{
	return this->logs;
}

const gsf::api_gsf::callbacks_container &gsf::api_gsf::get_callbacks() const
{
	return this->callbacks;
}

void gsf::api_gsf::push_log(std::string msg)
{
	this->logs.emplace_back(std::move(msg));
}

void gsf::api_gsf::_api_log(std::string txt)
{
	this->push_log(std::move(txt));
}

bool gsf::api_gsf::_api_register_callback(std::string id, sol::function callback)
{
	auto hashed_id = utils::hash_fnv1a(id.c_str());
	api_gsf::callback *arr_callbacks = reinterpret_cast<api_gsf::callback *>(&this->callbacks);
	for (int i = 0; i < sizeof(this->callbacks) / sizeof(callback); ++i)
	{
		if (arr_callbacks[i].active == false && hashed_id == arr_callbacks[i].hashed_name)
		{
			arr_callbacks[i].reg(callback);
			return true;
		}
	}

	this->push_log("Invalid callback ID: " + id);
	return false;
}