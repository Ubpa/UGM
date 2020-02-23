#pragma once

namespace Ubpa {
	namespace detail {
		template<typename M, size_t N>
		struct eye;

		template<typename M>
		struct eye<M, 3> {
			using F = typename M::F;
			static const M run() noexcept {
				return std::array<F, 3 * 3>{
					1, 0, 0,
					0, 1, 0,
					0, 0, 1,
				};
			}
		};

		template<typename M>
		struct eye<M, 4> {
			using F = typename M::F;

			static const M run() noexcept {
				return std::array<F, 4 * 4>{
						1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1
				};
			}
		};
	}
}