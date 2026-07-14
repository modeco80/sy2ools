#include "fk_utils.hpp"

#include <cstring>

namespace sly::sly2 {

	constexpr inline static auto sFkPrefixSize = sizeof("FK$") - 1;

	bool startsWithFkLookup(const char* pszInput) {
		return std::strncmp(pszInput, "FK$", sFkPrefixSize) == 0;
	}

	bool validFkLookup(const char* pszInput) {
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

	FileKind getFkLookupType(const char* pszInput) {
		return static_cast<FileKind>(pszInput[(sFkPrefixSize)]);
	}

	const char* getFkSearchName(const char* pszInput) {
		if(!validFkLookup(pszInput))
			return pszInput;
		return pszInput + (sFkPrefixSize + 1);
	}

	void makeFkSearchName(char* pszOutput, usize length, FileKind fk, const char* pszName) {
		std::snprintf(&pszOutput[0], length, "FK$%c%s", static_cast<char>(fk), pszName);
	}

} // namespace sly::sly2
