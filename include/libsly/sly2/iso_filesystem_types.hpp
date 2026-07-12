#pragma once
#include <mco/base_types.hpp>

namespace sly::sly2 {

	/// CD catalog entry. Exposed here since it is read in some other places.
	struct CdCatalogEntry {
		u32 lba;
		u32 size;
		u32 unk;
		u8 unk2[40]; // TODO this could be interesting data
		u32 lbaXorKey;
		u32 unkXorKey;
		u32 sizeXorKey;

		u32 getSize() const {
			return size ^ sizeXorKey;
		}

		u32 getLba() const {
			return (lba ^ lbaXorKey);
		}
	};

} // namespace sly::sly2
