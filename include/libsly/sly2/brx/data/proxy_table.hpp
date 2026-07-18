#pragma once
#include <mco/base_types.hpp>

namespace sly::sly2::brx {

	struct ProxyTableEntry {
		std::string name; // Wal OID/entity name
		i32 unk1;
		i32 unk2;
		i32 oid;
		i32 unk4;
		i32 unk5; // ?
	};

} // namespace sly::sly2::brx
