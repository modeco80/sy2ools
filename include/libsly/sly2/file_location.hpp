#pragma once
#include <libsly/sly2/iso_filesystem_types.hpp>
#include <variant>

namespace sly::sly2 {

	/// Class which abstracts file location.
	class FileLocation {
		using Variant = std::variant<
		std::monostate, // nothing
		std::string,	// FK$ WAL string
		CdCatalogEntry	// CD catalog
		>;

		Variant variant;

	   public:
		constexpr FileLocation() = default;
		FileLocation(const std::string& fkLookup);
		FileLocation(const CdCatalogEntry& cdCatalog);

		/// Returns a string repressentation. For debugging only.
		std::string toString() const;

		/// Returns true if this FileLocation is holding a FK$ lookup string.
		bool isFkLookup() const;

		/// Returns true if this FileLocation is holding a CD catalog entry.
		bool isCdCatalog() const;

		// getters
		const std::string& getFkLookup() const;
		const CdCatalogEntry& getCdCatalog() const;
	};

} // namespace sly::sly2
