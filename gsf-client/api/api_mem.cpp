#include "api_mem.h"

#include <mem.h>
#include <Windows.h>

bool gsf::api_mem::setup_api()
{
	return false;
}

sol::object gsf::api_mem::_api_ida_scan(std::uintptr_t start_adr, std::size_t size, std::string ida_pattern)
{
	auto result = utils::ida_scan(reinterpret_cast<void *>(start_adr), size, ida_pattern.c_str());

	if (!result)
		return sol::nil;

	return sol::make_object(this->get_lua_state(), reinterpret_cast<std::uintptr_t>(result));
}

int gsf::api_mem::_api_patch(std::uintptr_t addr, std::vector<std::uint8_t> byte_array)
{
	enum result_e
	{
		SUCCESSFUL = 0,
		ALREADY_PATCHED = 1,
		PROT_CHANGE_XRW_FAILED = 2,
		PROT_CHANGE_RESTORE_FAILED = 3
	};

	// TODO: store in a global static list all the patched memory and check for collisions
	// TODO: use win api helper

	void *addr_void = reinterpret_cast<void *>(addr);
	std::size_t byte_arr_size = byte_array.size();
	DWORD old_prot = 0;

	if (!VirtualProtect(addr_void, byte_arr_size, PAGE_EXECUTE_READWRITE, &old_prot))
	{
		this->push_log("mem.patch # exception: PROT_CHANGE_XRW_FAILED");
		return result_e::PROT_CHANGE_XRW_FAILED;
	}

	std::memcpy(addr_void, byte_array.data(), byte_arr_size);

	if (!VirtualProtect(addr_void, byte_arr_size, old_prot, &old_prot))
	{
		this->push_log("mem.patch # exception: PROT_CHANGE_RESTORE_FAILED");
		return result_e::PROT_CHANGE_RESTORE_FAILED;
	}

	return result_e::SUCCESSFUL;
}

std::uint64_t gsf::api_mem::_api_read_uint(std::uintptr_t addr, std::size_t prim_t_size)
{
	if (prim_t_size > 8)
	{
		this->push_log("mem.read_uint # parameter prim_t_size is out of bound");
		return -1;
	}

	std::uint64_t result = 0;
	std::memcpy(&result, reinterpret_cast<void *>(addr), prim_t_size);
	return result;
}

void gsf::api_mem::_api_write_uint(std::uintptr_t addr, std::size_t prim_t_size, std::uint64_t value)
{
	if (prim_t_size > 8)
	{
		this->push_log("mem.write_uint # parameter prim_t_size is out of bound");
	}

	std::memcpy(reinterpret_cast<void *>(addr), &value, prim_t_size);
}

std::uintptr_t gsf::api_mem::_api_calc_rel_address_32(std::uintptr_t inst_addr, std::size_t inst_size)
{
	return reinterpret_cast<std::uintptr_t>(utils::calc_rel_address_32(reinterpret_cast<void *>(inst_addr), inst_size));
}
