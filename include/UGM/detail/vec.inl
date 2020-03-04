#pragma once

namespace Ubpa::detail::Basic {
	template<typename T, size_t N>
	struct rmv_epsilon<vec<T, N>> {
		static const vec<T, N> run(const vec<T, N>& x) noexcept {
			return x.rmv_epsilon();
		}
	};

	template<typename T, size_t N>
	struct is_zero<vec<T, N>> {
		static bool run(const vec<T, N>& x) noexcept {
			return x.is_all_zero();
		}
	};

	template<typename T, size_t N>
	struct is_nan<vec<T, N>> {
		static bool run(const vec<T, N>& x) noexcept {
			return x.has_nan();
		}
	};
}
