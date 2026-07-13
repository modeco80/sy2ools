#include "utils.hpp"

#include <libsly/sly2/iso_filesystem.hpp>
#include <mco/io/file_stream.hpp>

namespace slyarc {

	std::unique_ptr<sly::sly2::IArchiveFileSystem> createArchiveFs(const std::filesystem::path& path) {
		if(!path.has_extension())
			throw std::runtime_error("Can't create filesystem for this at the moment");

		if(path.extension().string() == ".iso") {
			auto fs = mco::FileStream::open(path.string().c_str(), mco::FileStream::Read);
			auto iso = std::make_unique<sly::sly2::IsoFileSystem>(std::move(fs));
			return iso;
		}

		// TODO: WAC/WAL support.
		throw std::runtime_error("Can't create filesystem for this at the moment");
	}
} // namespace slyarc
