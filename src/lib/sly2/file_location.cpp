#include <libsly/sly2/file_location.hpp>
#include <format>
#include <stdexcept>

#include "fk_utils.hpp"

namespace sly::sly2 {

	template <typename VariantType, typename T, std::size_t Index = 0>
	consteval std::size_t typeIndexFor() {
		static_assert(Index < std::variant_size_v<VariantType>, "Type not found in variant!");

		if constexpr(std::is_same_v<std::variant_alternative_t<Index, VariantType>, T>) {
			return Index;
		} else {
			return typeIndexFor<VariantType, T, Index + 1>();
		}
	}

	FileLocation::FileLocation(const std::string& fkLookup) {
		// TODO throw better exception
		if(!validFkLookup(fkLookup.c_str()))
			throw std::runtime_error("Invalid FK$ lookup string");
		variant = fkLookup;
	}

	FileLocation::FileLocation(const CdCatalogEntry& cdCatalog) {
		variant = cdCatalog;
	}

	std::string FileLocation::toString() const {
		switch(variant.index()) {
			case typeIndexFor<Variant, std::string>():
				return std::format("FK lookup: {}", std::get<std::string>(variant));
				break;
			case typeIndexFor<Variant, CdCatalogEntry>(): {
				const auto& a = std::get<CdCatalogEntry>(variant);
				return std::format("Cd catalog: LBA Base {} Size {}", a.getLba(), a.getSize());
			} break;
			default:
				throw std::runtime_error("In invalid state");
				break;
		}
	}

	bool FileLocation::isFkLookup() const {
		return variant.index() == typeIndexFor<Variant, std::string>();
	}

	bool FileLocation::isCdCatalog() const {
		return variant.index() == typeIndexFor<Variant, CdCatalogEntry>();
	}

	// getters
	const std::string& FileLocation::getFkLookup() const {
		if(!isFkLookup())
			throw std::runtime_error("Not a FK lookup");
		return std::get<std::string>(variant);
	}

	const CdCatalogEntry& FileLocation::getCdCatalog() const {
		if(!isCdCatalog())
			throw std::runtime_error("Not a CD catalog");
		return std::get<CdCatalogEntry>(variant);
	}
} // namespace sly::sly2
