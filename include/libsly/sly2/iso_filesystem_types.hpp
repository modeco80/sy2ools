#pragma once
#include <mco/base_types.hpp>

namespace sly::sly2 {

	/// CD catalog entry. Exposed here since it is read in some other places.
	struct CdCatalogEntry {
		u32 lba;
		u32 size;
		u32 id;
		u8 garbage[40];
		u32 lbaXorKey;
		u32 idXorKey;
		u32 sizeXorKey;

		u32 getSize() const {
			return size ^ sizeXorKey;
		}

		u32 getLba() const {
			return (lba ^ lbaXorKey);
		}

		u32 getId() const {
			return (id ^ idXorKey);
		}
	};

	mcoAssertSize(CdCatalogEntry, 0x40);

} // namespace sly::sly2
