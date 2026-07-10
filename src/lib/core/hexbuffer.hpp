#pragma once
#include <array>
#include <mco/base_types.hpp>
#include <mco/fixed_string.hpp>

namespace sly::impl {

	consteval u8 parseHexNibble(char c) {
		if(c >= '0' && c <= '9') {
			return c - '0';
		} else if(c >= 'A' && c <= 'F') {
			return c - 'A' + 10;
		} else if(c >= 'a' && c <= 'f') {
			return c - 'a' + 10;
		}
		throw "Invalid hex nibble";
	}

	/// Converts a hex string into a fixed buffer at compile time.
	template <mco::FixedString inputHexString>
	consteval auto hexToBuffer() {
		if(inputHexString.length() % 2) {
			throw "invalid input";
		}

		std::array<u8, inputHexString.length() / 2> buf;
		for(auto i = 0; i < inputHexString.length(); i += 2)
			buf[i / 2] = (parseHexNibble(inputHexString[i]) << 4) | parseHexNibble(inputHexString[i + 1]);
		return buf;
	}

	static_assert(hexToBuffer<"fefa">()[0] == 0xfe, "hexToBuffer is broken. FIX IT!");
	static_assert(hexToBuffer<"fefa">()[1] == 0xfa, "hexToBuffer is broken. FIX IT!");
} // namespace sly::impl
