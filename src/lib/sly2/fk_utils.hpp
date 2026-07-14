//! Utilities for parsing & creating wal FK$ lookup strings.
#pragma once

#include <libsly/sly2/archive_filesystem.hpp>

namespace sly::sly2 {

	/// Returns whether or not a string starts with the FK$ WAL lookup prefix.
	bool startsWithFkLookup(const char* pszInput);

	/// Returns true if this is a valid FK$ lookup string.
	bool validFkLookup(const char* pszInput);

	/// Gets the type (FileKind) of a FK$ lookup.
	/// NOTE: Must use [validFkLookup()] first!
	FileKind getFkLookupType(const char* pszInput);

	/// Gets the search name (file name, essentially) of a FK$ lookup.
	const char* getFkSearchName(const char* pszInput);

	/// Creates a FK$ search name.
	void makeFkSearchName(char* pszOutput, usize length, FileKind fk, const char* pszName);

} // namespace sly::sly2
