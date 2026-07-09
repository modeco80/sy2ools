#include <libsly/core/buffer_streams.hpp>
#include <mco/io/file_stream.hpp>
#include <mco/io/stream_utils.hpp>
#include <optional>
#include <string_view>
#include <vector>

namespace versions {
	struct Usa {
		constexpr static u32 lbaElfBase = 0x10f;
		constexpr static u32 lbaCdDataBase = 3232;				 // Hard-coded data start
		constexpr static u32 countCdCatalog = 30;				 // amount of catalog files
		constexpr static u32 offsetElfCdCatalogTable = 0x1a32e8; // in elf.

		constexpr static std::optional<std::string_view> getFileNameFromIndex(u32 index) {
			switch(index) {
				case 24:
					return "splash.pss";
				case 25:
					return "intro.brx";
				case 26:
					return "memcard_icon.ico";
				default:
					return std::nullopt;
			}
		}
	};
} // namespace versions

constexpr u64 lbaToOffset(u32 lba) {
	return static_cast<u64>(lba) * 0x800;
}

/// CD catalog entry
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

	template <class Version = versions::Usa>
	u32 getLba() const {
		return (lba ^ lbaXorKey) + Version::lbaCdDataBase;
	}
};

template <class Version = versions::Usa>
std::vector<CdCatalogEntry> readMainCdCatalog(mco::Stream& stream) {
	std::vector<CdCatalogEntry> ret;
	ret.resize(Version::countCdCatalog);

	// Read the catalog
	auto nCatalogBytes = Version::countCdCatalog * sizeof(CdCatalogEntry);
	stream.seek(lbaToOffset(Version::lbaElfBase) + Version::offsetElfCdCatalogTable, mco::Stream::Begin);
	if(stream.read(ret.data(), nCatalogBytes) != nCatalogBytes) {
		throw std::runtime_error("Short read of CD root catalog. This should NEVER happen!");
	}

	return ret;
}

template <class Version = versions::Usa>
void dump(mco::FileStream& isoStream) {
	auto catalog = readMainCdCatalog<Version>(isoStream);
	u32 i = 0;

	for(auto& entry : catalog) {
		char name[32] {};
		if(auto sv = Version::getFileNameFromIndex(i); sv) {
			std::sprintf(&name[0], "dump/%s", (*sv).data());
		} else {
			std::sprintf(&name[0], "dump/%d.bin", i);
		}

		{
			auto fsWrite = mco::FileStream::open(name, mco::FileStream::Create | mco::FileStream::ReadWrite);
			auto tmp = isoStream.seekTemporary(lbaToOffset(entry.getLba()), mco::Stream::Begin);

			// Dump the file to disk. This uses buffered adapter streams to increase performance slightly.
			// (while teeStreams() does buffer data internally the performance increase of buffering the streams themselves
			// is still quite noticable.)
			auto bufferRead = sly::core::BufferedReadStream(isoStream, 0x2000);
			auto bufferWrite = sly::core::BufferedWriteStream(fsWrite, 0x2000);
			mco::teeStreams(bufferRead, bufferWrite, entry.getSize());
		}

		std::printf("LBA %d size %d written to %s\n", entry.getLba(), entry.getSize(), name);
		i++;
	}
}

int main(int argc, char** argv) {
	auto isoFileStream = mco::FileStream::open("sy2.iso", mco::FileStream::Read);
	dump<versions::Usa>(isoFileStream);
	return 0;
}
