#pragma once

namespace Ubpa::detail::IMatrix {
	
	template<typename M, size_t N>
	struct eye;

	template<typename M>
	struct eye<M, 3> {
		using F = typename M::F;
		inline static const M run() noexcept {
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

		inline static const M run() noexcept {
			return std::array<F, 4 * 4>{
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1
			};
		}
	};

	template<size_t N>
	struct transpose;

	template<>
	struct transpose<3> {
		template<typename M>
		inline static const M run(M& m) noexcept {
			static_assert(M::N == 3);

			return std::array<typename M::F, 3 * 3>{
				m(0,0), m(1,0), m(2,0),
				m(0,1), m(1,1), m(2,1),
				m(0,2), m(1,2), m(2,2)
			};
		}
	};

	template<>
	struct transpose<4> {
		template<typename M>
		inline static const M run(M& m) noexcept {
			static_assert(M::N == 4);

			return std::array<typename M::F, 4 * 4>{
				m(0, 0), m(1, 0), m(2, 0), m(3, 0),
				m(0, 1), m(1, 1), m(2, 1), m(3, 1),
				m(0, 2), m(1, 2), m(2, 2), m(3, 2),
				m(0, 3), m(1, 3), m(2, 3), m(3, 3)
			};
		}
	};
}