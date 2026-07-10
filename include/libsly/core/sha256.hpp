#pragma once
#include <array>
#include <mco/base_types.hpp>
#include <mco/io/stream.hpp>

namespace sly::core {
	/// The resulting bytes of a SHA-256 digest.
	using ShaDigest = std::array<u8, 32>;

	/// Performs a SHA-256 digest on the given bytes.
	ShaDigest sha256Digest(const u8* pData, usize size);

	/// Performs a SHA-256 digest on a mcolib stream.
	ShaDigest sha256Digest(mco::Stream& stream);

	/// Performs a SHA-256 digest on a mcolib stream, digesting only [nDigest] bytes of it.
	ShaDigest sha256Digest(mco::Stream& stream, usize nDigest);

} // namespace sly::core
