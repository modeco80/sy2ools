#include "io_utils.hpp"

#include <libsly/sly2/iso_filesystem.hpp>

namespace sly::sly2::brx {

	void readAll(mco::Stream& stream, void* pBuf, usize count) {
		if(stream.read(pBuf, count) != count)
			throw ShortRead();
	}

	std::string readSwString(mco::Stream& stream) {
		std::string ret;
		const auto nStringLen = readLiteral<i16>(stream);
		ret.resize(nStringLen);
		readAll(stream, &ret[0], nStringLen);
		return ret;
	}

	FileLocation readLocation(mco::Stream& stream, IArchiveFileSystem& fs) {
		if(fs.getKind() == IsoFileSystem::Kind) {
			auto& isoFs = reinterpret_cast<IsoFileSystem&>(fs);
			CdCatalogEntry catEnt;
			readAll(stream, &catEnt, sizeof(CdCatalogEntry));

			// IsoFileSystem contains a helper method to map CD catalogs back to
			// FK$ if it knows about them, so use it. This allows locations read here
			// to become FK$ as soon as the mappings are updated, which is pretty cool.
			return isoFs.mapCatalogEntryToName(catEnt);
		} else {
			// FK$ lookup string.
			char bufLoc[0x40] {};
			readAll(stream, &bufLoc[0], sizeof(bufLoc));
			return FileLocation(bufLoc);
		}
	}

} // namespace sly::sly2::brx
