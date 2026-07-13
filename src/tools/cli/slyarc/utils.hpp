#pragma once

#include <filesystem>
#include <libsly/sly2/archive_filesystem.hpp>
#include <memory>

namespace slyarc {

	/// Helper which creates a given libsly [IArchiveFileSystem] for the path.
	std::unique_ptr<sly::sly2::IArchiveFileSystem> createArchiveFs(const std::filesystem::path& path);

} // namespace slyarc
