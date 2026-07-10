#include <format>
#include <libsly/sly2/release.hpp>

#include "../core/hexbuffer.hpp"

namespace sly::sly2 {

	constexpr static const char* regionToString(Region region) {
		using enum Region;
		switch(region) {
			case NtscU:
				return "NTSC-U";
			case Invalid:
			default:
				return "Invalid";
		}
	}

	constexpr static const char* versionToString(Version version) {
		using enum Version;
		switch(version) {
			case Release:
				return "Release";
			case ProtoMarch:
				return "March 17, 2004 Prototype";
			case ProtoE3:
				return "April 12, 2004 E3 Prototype";
			default:
				return "???";
		}
	}

	// Declare digests here.
#define X(region, version, sha256) \
	constexpr static auto sGameDigest_##region##_##version = sly::impl::hexToBuffer<sha256>();
	LIBSLY_SLY2_RELEASES()
#undef X

	const core::ShaDigest* getDigestForRelease(Release release) {
		using enum Release;
		switch(release) {
#define X(region, version, _sha256) \
	case region##_##version:        \
		return &sGameDigest_##region##_##version;
			LIBSLY_SLY2_RELEASES()
#undef X
			case Invalid:
			default:
				return nullptr;
		}
	}

	std::string getReleaseString(Release release) {
		if(release == Release::Invalid)
			return "Invalid";
		return std::format("{} {}", regionToString(extractGameRegion(release)), versionToString(extractGameVersion(release)));
	}
} // namespace sly::sly2
