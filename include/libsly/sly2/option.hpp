#pragma once
#include <libsly/sly2/types.hpp>
#include <libsly/sly2/vector.hpp>
#include <mco/base_types.hpp>
#include <mco/io/stream.hpp>
#include <vector>

namespace sly::sly2 {

#define LIBSLY_OPTION_TYPES()                  \
	X(Bool, 0, bool, boolVal)                  \
	X(Float, 1, float, floatVal)               \
	X(Vector, 2, sly2::Vector, vecVal)         \
	X(Int, 5, i32, i32Val)                     \
	X(Smpa, 0xc, SmoothingParameters, smpaVal) \
	X(Rgba, 0xd, Rgba, rgbaVal)                \
	X(Wid, 0xf, Wid, widVal)

	enum class OptionType : u32 {
#define X(key, value, _T, _member) \
	key = value,
		LIBSLY_OPTION_TYPES()
#undef X
		Void = 0x12,
	};

	namespace impl {

		template <class T>
		struct TypeToOptionType {};

#define OPTION_TYPE_MAPPING(otyp, T)                  \
	template <>                                       \
	struct TypeToOptionType<T> {                      \
		constexpr static auto Opt = OptionType::otyp; \
	}

#define X(kind, _, T, __) \
	OPTION_TYPE_MAPPING(kind, T);
		LIBSLY_OPTION_TYPES()
#undef X

#undef OPTION_TYPE_MAPPING

		template <class T>
		constexpr inline static auto TypeToOptionTypeT = TypeToOptionType<T>::Opt;

		class OptionValue {
			OptionType type;
			union {
#define X(_k, _v, T, member) \
	struct {                 \
		T member;            \
	};
				LIBSLY_OPTION_TYPES()
#undef X
			};

		   public:
			OptionValue();

#define X(_k, _v, T, _m) \
	OptionValue(T value);
			LIBSLY_OPTION_TYPES()
#undef X

			OptionValue(const OptionValue&);
			// move ctor? or would that not be worth the pain

			constexpr bool matchesType(OptionType type) const {
				return this->type == type;
			}

			template <class T>
			constexpr bool matchesType() const {
				return matchesType(TypeToOptionTypeT<T>);
			}

			template <class T>
			constexpr auto& get() const {
				if(!matchesType<T>())
					throw std::runtime_error("Invalid type lookup!!");

#define X(_key, _v, type, member)         \
	if constexpr(std::is_same_v<T, type>) \
		return member;
				LIBSLY_OPTION_TYPES()
#undef X
			}
		};

	} // namespace impl

	using impl::OptionValue;

	struct OptionDescriptor {
		const char* pszName;
		bool bSpliceCall; // splice option calls are included in the opid lists.
		OptionType type;
		// TODO w/ splice options:
		// - call
		// - get
		// - set
	};

} // namespace sly::sly2
