#pragma once

namespace Ubpa::detail::IMatrix_ {
	
	template<typename M, size_t N>
	struct eye;

	template<typename M>
	struct eye<M, 3> {
		using F = typename M::F;
		inline static const M run() noexcept {
			return {
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
			return {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};
		}
	};

	// =======================================================

	template<size_t N>
	struct transpose;

	template<>
	struct transpose<3> {
		template<typename M>
		inline static const M run(const M& m) noexcept {
			static_assert(M::N == 3);

			return {
				m(0,0), m(1,0), m(2,0),
				m(0,1), m(1,1), m(2,1),
				m(0,2), m(1,2), m(2,2)
			};
		}
	};

	template<>
	struct transpose<4> {
		template<typename M>
		inline static const M run(const M& m) noexcept {
			static_assert(M::N == 4);
#ifdef UBPA_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<ImplTraits_T<M>>) {
				M rst{ m };
				_MM_TRANSPOSE4_PS(rst[0], rst[1], rst[2], rst[3]);
				return rst;
			}
			else
#endif // UBPA_USE_SIMD
			{
				return {
					m(0, 0), m(1, 0), m(2, 0), m(3, 0),
					m(0, 1), m(1, 1), m(2, 1), m(3, 1),
					m(0, 2), m(1, 2), m(2, 2), m(3, 2),
					m(0, 3), m(1, 3), m(2, 3), m(3, 3)
				};
			}
		}
	};

	// =======================================================

	template<size_t N>
	struct trace;

	template<>
	struct trace<3> {
		template<typename M>
		inline static ImplTraits_F<M> run(const M& m) noexcept {
			static_assert(M::N == 3);

			return m[0][0] + m[1][1] + m[2][2];
		}
	};

	template<>
	struct trace<4> {
		template<typename M>
		inline static ImplTraits_F<M> run(const M& m) noexcept {
			static_assert(M::N == 4);
#ifdef UBPA_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<ImplTraits_T<M>>)
				return m[0].get<0>() + m[1].get<1>() + m[2].get<2>() + m[3].get<3>();
			else
#endif // UBPA_USE_SIMD
				return m[0][0] + m[1][1] + m[2][2] + m[3][3];
		}
	};

	// =======================================================

	template<size_t N>
	struct init;

	template<>
	struct init<3> {
		template<typename M>
		inline static void run(M& m, const std::array<ImplTraits_F<M>, 3 * 3>& data) noexcept {
			static_assert(M::N == 3);
			memcpy(&m, data.data(), 9 * sizeof(ImplTraits_F<M>));
		}
	};

	template<>
	struct init<4> {
		template<typename M>
		inline static void run(M& m, const std::array<ImplTraits_F<M>, 4 * 4>& data) noexcept {
			static_assert(M::N == 4);
#ifdef UBPA_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<ImplTraits_T<M>>) {
				m[0] = _mm_loadu_ps(&(data[0]));
				m[1] = _mm_loadu_ps(&(data[4]));
				m[2] = _mm_loadu_ps(&(data[8]));
				m[3] = _mm_loadu_ps(&(data[12]));
			}
			else
#endif // UBPA_USE_SIMD
			{
				memcpy(&m, data.data(), 16 * sizeof(ImplTraits_F<M>));
			}
		}
	};

	// =======================================================

	template<size_t N>
	struct zero;

	template<>
	struct zero<3> {
		template<typename M>
		inline static M run() noexcept {
			static_assert(M::N == 3);
			return {
				0,0,0,
				0,0,0,
				0,0,0
			};
		}
	};

	template<>
	struct zero<4> {
		template<typename M>
		inline static M run() noexcept {
			static_assert(M::N == 4);
#ifdef UBPA_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<ImplTraits_T<M>>) {
				using V = ImplTraits_T<M>;
				const __m128 z = _mm_set1_ps(0.f);
				return { V{z}, V{z}, V{z}, V{z} };
			}
			else
#endif // UBPA_USE_SIMD
			{
				return {
					0,0,0,0,
					0,0,0,0,
					0,0,0,0,
					0,0,0,0
				};
			}
		}
	};
}