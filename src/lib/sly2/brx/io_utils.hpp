#pragma once
#include <libsly/sly2/archive_filesystem.hpp>
#include <libsly/sly2/file_location.hpp>
#include <mco/io/stream.hpp>
#include <vector>

namespace sly::sly2::brx {

	// IO helpers
	// TODO: might be useful in another file.

	class ShortRead : public std::exception {
	   public:
		const char* what() const noexcept override {
			return "short read";
		}
	};

	void readAll(mco::Stream& stream, void* pBuf, usize count);

	template <class T>
	T readLiteral(mco::Stream& stream) {
		T item;
		readAll(stream, &item, sizeof(T));
		return item;
	}

	std::string readSwString(mco::Stream& stream);
	template <class TCountInt, class Out, class FnRead>
	void readArray(mco::Stream& stream, std::vector<Out>& array, FnRead&& doRead) {
		const auto nCount = readLiteral<TCountInt>(stream);
		array.resize(nCount);
		for(usize i = 0; i < nCount; ++i)
			doRead(stream, array[i]);
	}

	FileLocation readLocation(mco::Stream& stream, IArchiveFileSystem& fs);

} // namespace sly::sly2::brx
