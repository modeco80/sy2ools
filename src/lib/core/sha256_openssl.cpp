#include <openssl/err.h>
#include <openssl/evp.h>

#include <libsly/core/sha256.hpp>

namespace sly::core {

	class EvpContext {
		EVP_MD_CTX* pCtx;

	   public:
		EvpContext() {
			pCtx = EVP_MD_CTX_new();
			if(pCtx == nullptr)
				throw std::bad_alloc();
		}

		EvpContext(const EvpContext& context) = delete;

		EvpContext(EvpContext&& mv) {
			pCtx = mv.pCtx;
			mv.pCtx = nullptr;
		}

		~EvpContext() {
			if(pCtx) {
				EVP_MD_CTX_free(pCtx);
			}
		}

		void digestInit(const EVP_MD* methodTable, ENGINE* engine = nullptr) {
			EVP_DigestInit_ex(pCtx, methodTable, engine);
		}

		void digestUpdate(const u8* pData, usize length) {
			EVP_DigestUpdate(pCtx, pData, length);
		}

		void digestFinal(u8* pDigest) {
			EVP_DigestFinal_ex(pCtx, pDigest, nullptr);
		}
	};

	ShaDigest sha256Digest(const u8* pData, usize length) {
		ShaDigest digest {};
		EvpContext ctx;
		ctx.digestInit(EVP_sha256());
		ctx.digestUpdate(pData, length);
		ctx.digestFinal(&digest[0]);
		return digest;
	}

	constexpr static auto kDigestStreamChunkSize = 2048;

	ShaDigest sha256Digest(mco::Stream& stream) {
		ShaDigest digest {};
		u8 buffer[kDigestStreamChunkSize] {};

		EvpContext ctx;
		ctx.digestInit(EVP_sha256());

		while(true) {
			auto n = stream.read(&buffer[0], sizeof(buffer));
			if(n == 0)
				break;
			ctx.digestUpdate(&buffer[0], n);
		}

		ctx.digestFinal(&digest[0]);
		return digest;
	}

	ShaDigest sha256Digest(mco::Stream& stream, usize nDigest) {
		u8 buffer[kDigestStreamChunkSize] {};
		ShaDigest digest {};
		EvpContext ctx;
		ctx.digestInit(EVP_sha256());

		// Calculate how many chunks it will take to tee the amount of bytes
		// we have been requested to.
		//
		// Since it may not be evenly divisible into chunks,
		// we also need to calculate the remainder
		u64 nChunks = nDigest / kDigestStreamChunkSize;
		u64 nRemainder = nDigest % kDigestStreamChunkSize;

		// Do as much as we can in chunks of [kDigestStreamChunkSize] bytes.
		for(u64 i = 0; i < nChunks; ++i) {
			auto n = stream.read(&buffer[0], kDigestStreamChunkSize);
			ctx.digestUpdate(&buffer[0], n);
		}

		// Once we've written all we can as chunks, we need to
		// write the final remainder, if there is any.
		if(nRemainder) {
			auto n = stream.read(&buffer[0], nRemainder);
			ctx.digestUpdate(&buffer[0], n);
		}

		ctx.digestFinal(&digest[0]);
		return digest;
	}
} // namespace sly::core
