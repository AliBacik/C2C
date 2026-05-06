#pragma once
#include <vector>
#include <string>
#include "crypto.h"

namespace cfg
{
	static std::string kmVersion = xorstr_("1");
	static std::string umVersion = xorstr_("1");
	static std::string name    = xorstr_("1");
	static std::string author  = xorstr_("1");

	static std::vector<uint8_t> key = {};

	static std::vector<uint8_t> image = {};

	static std::vector<uint8_t> imageLegacy = {};
}