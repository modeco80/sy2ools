#pragma once
#include <libsly/core/sha256.hpp>
#include <mco/base_types.hpp>

namespace sly::sly2 {

	namespace impl {

		enum class Region : u8 {
			Invalid = 0xff,
			NtscU = 0,
		};

		enum class Version : u8 {
			Invalid = 0xff,

			// Release versions
			// NOTE: This has a carved in space to allow post-rtm revisions..
			Release = 0,

			// PROTOTYPE VERSIONS
			ProtoMarch = 10,
			ProtoE3,
			Proto
		};

		// This X-macro invocation directly maps all supported Sly2 releases.
		//
		// When adding recognition into libsly/sly2, you'll need to add to this. Also src/lib/sly2/iso_filemapping.cpp.
		// Note that the digest here is of the first 1MB of the ISO itself.
		//
		// To generate it, on linux, you can do something like
		// "dd if=/path/to/mysy2.iso bs=1048576 count=1 | sha256sum -b -"
#define LIBSLY_SLY2_RELEASES() \
	X(NtscU, Release, "68e6f0d8b007fda76057a156a889eaafe51f1ebd5d6dcd018f8d895437216f5c")

	} // namespace impl

	using impl::Region;
	using impl::Version;

	// Strongly typed enum.
	enum class Release : u16 {
		Invalid = 0xffff,

#define X(region, version, _sha256) \
	region##_##version = (static_cast<u16>(Region::region) << 8) | static_cast<u16>(Version::version)
		LIBSLY_SLY2_RELEASES()
#undef X
	};

	constexpr Version extractGameVersion(Release version) {
		return static_cast<Version>(static_cast<u8>(static_cast<u16>(version) & 0xff));
	}

	constexpr Region extractGameRegion(Release version) {
		return static_cast<Region>(static_cast<u8>((static_cast<u16>(version) >> 8) & 0xff));
	}

	template <class F>
	auto forEachGameRelease(F&& f) {
// Effectively this generates a unrolled loop over all valid versions.
#define X(region, version, _sha256)             \
	if(f(Release::region##_##version) == false) \
		return;
		LIBSLY_SLY2_RELEASES()
#undef X
	}

	const core::ShaDigest* getDigestForRelease(Release release);

	std::string getReleaseString(Release release);

	// Make sure the above functions function properly at compile time. Yay for compile-time unit testing.
	static_assert(extractGameRegion(Release::NtscU_Release) == Region::NtscU, "extractGameRegion() needs fixed");
	static_assert(extractGameVersion(Release::NtscU_Release) == Version::Release, "extractGameVersion() needs fixed");

} // namespace sly::sly2
