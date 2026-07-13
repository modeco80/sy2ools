//! Utilities for parsing wal FK$ lookup strings.
#pragma once

#include <cstring>
#include <libsly/sly2/archive_filesystem.hpp>
#include <optional>

namespace sly::sly2 {
	namespace impl {

		constexpr inline static auto sFkPrefixSize = sizeof("FK$") - 1;

		/// Returns whether or not a string starts with the FK$ WAL lookup prefix.
		inline bool startsWithFkLookup(const char* pszInput) {
			return std::strncmp(pszInput, "FK$", sFkPrefixSize) == 0;
		}

		/// Returns true if this is a valid FK$ lookup string.
		inline bool validFkLookup(const char* pszInput) {
			if(pszInput == nullptr)
				return false;

			if(!startsWithFkLookup(pszInput))
				return false;

			// make sure someone isn't being stupid
			if(pszInput[(sFkPrefixSize)] == '\0')
				return false;

			// There must be a search string after.
			if(pszInput[(sFkPrefixSize + 1)] == '\0')
				return false;

			// it is (presumably) a valid enough fk lookup?
			return true;
		}

		/// Gets the type (FileKind) of a FK$ lookup.
		/// NOTE: Must use [validFkLookup()] first!
		inline FileKind getFkLookupType(const char* pszInput) {
			return static_cast<FileKind>(pszInput[(sFkPrefixSize)]);
		}

		/// Gets the search name (file name, essentially) of a FK$ lookup.
		inline const char* getFkSearchName(const char* pszInput) {
			if(!validFkLookup(pszInput))
				return pszInput;
			return pszInput + (sFkPrefixSize + 1);
		}
	} // namespace impl

	using impl::getFkLookupType;
	using impl::getFkSearchName;
	using impl::startsWithFkLookup;
	using impl::validFkLookup;

} // namespace sly::sly2
