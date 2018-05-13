#pragma once

#include "picosha2.h"
#include <string>

struct Hash256
{
	static const size_t size = 32;
	uint8_t bytes[size];

	void HashFromString(const std::string& content)
	{
		picosha2::hash256(content, bytes, bytes + size);
	}

	void HashFromByteStreams(const std::vector<uint8_t> s)
	{
		picosha2::hash256(s.begin(), s.end(), std::begin(bytes), std::end(bytes));
	}

	std::string& ToString() const
	{
		static std::string hex_string;
		picosha2::hash256_hex_string(bytes, bytes + size, hex_string);
		hex_string = hex_string.substr(0, 8);
		return hex_string;
	}
};
