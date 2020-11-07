#pragma once

#include "svd3.h"

namespace Ubpa::details::IMatrix_ {
	template<typename M, size_t N>
	struct eye;

	template<typename M>
	struct eye<M, 2> {
		using F = typename M::F;
		inline static const M run() noexcept {
			return {
				1, 0,
				0, 1,
			};
		}
	};

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
	struct det;

	template<>
	struct det<2> {
		template<typename M>
		inline static typename M::F run(const M& m) noexcept {
			using F = typename M::F;
			return m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0);
		}
	};

	template<>
	struct det<3> {
		template<typename M>
		inline static typename M::F run(const M& m) noexcept {
			using F = typename M::F;
			return m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
				+ m[1][0] * (m[2][1] * m[0][2] - m[0][1] * m[2][2])
				+ m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]);
		}
	};

	template<>
	struct det<4> {
		template<typename M>
		inline static typename M::F run(const M& m) noexcept {
			using F = typename M::F;

			F r00 = m[1][1] * m[2][2] * m[3][3] -
				m[1][1] * m[2][3] * m[3][2] -
				m[2][1] * m[1][2] * m[3][3] +
				m[2][1] * m[1][3] * m[3][2] +
				m[3][1] * m[1][2] * m[2][3] -
				m[3][1] * m[1][3] * m[2][2];

			F r10 = -m[1][0] * m[2][2] * m[3][3] +
				m[1][0] * m[2][3] * m[3][2] +
				m[2][0] * m[1][2] * m[3][3] -
				m[2][0] * m[1][3] * m[3][2] -
				m[3][0] * m[1][2] * m[2][3] +
				m[3][0] * m[1][3] * m[2][2];

			F r20 = m[1][0] * m[2][1] * m[3][3] -
				m[1][0] * m[2][3] * m[3][1] -
				m[2][0] * m[1][1] * m[3][3] +
				m[2][0] * m[1][3] * m[3][1] +
				m[3][0] * m[1][1] * m[2][3] -
				m[3][0] * m[1][3] * m[2][1];

			F r30 = -m[1][0] * m[2][1] * m[3][2] +
				m[1][0] * m[2][2] * m[3][1] +
				m[2][0] * m[1][1] * m[3][2] -
				m[2][0] * m[1][2] * m[3][1] -
				m[3][0] * m[1][1] * m[2][2] +
				m[3][0] * m[1][2] * m[2][1];

			F det = m[0][0] * r00 + m[0][1] * r10 + m[0][2] * r20 + m[0][3] * r30;
		}
	};

	// =======================================================

	template<size_t N>
	struct transpose;

	template<>
	struct transpose<2> {
		template<typename M>
		inline static const M run(const M& m) noexcept {
			static_assert(M::N == 2);

			return {
				m(0,0), m(1,0),
				m(0,1), m(1,1),
			};
		}
	};

	template<>
	struct transpose<3> {
		template<typename M>
		inline static const M run(const M& m) noexcept {
			static_assert(M::N == 3);

			return {
				m(0,0), m(1,0), m(2,0),
				m(0,1), m(1,1), m(2,1),
				m(0,2), m(1,2), m(2,2),
			};
		}
	};

	template<>
	struct transpose<4> {
		template<typename M>
		inline static const M run(const M& m) noexcept {
			static_assert(M::N == 4);
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<ImplTraits_T<M>>) {
				M rst{ m };
				_MM_TRANSPOSE4_PS(rst[0], rst[1], rst[2], rst[3]);
				return rst;
			}
			else
#endif // UBPA_UGM_USE_SIMD
			{
				return {
					m(0, 0), m(1, 0), m(2, 0), m(3, 0),
					m(0, 1), m(1, 1), m(2, 1), m(3, 1),
					m(0, 2), m(1, 2), m(2, 2), m(3, 2),
					m(0, 3), m(1, 3), m(2, 3), m(3, 3),
				};
			}
		}
	};

	// =======================================================

	template<size_t N>
	struct trace;

	template<>
	struct trace<2> {
		template<typename M>
		inline static ImplTraits_F<M> run(const M& m) noexcept {
			static_assert(M::N == 2);

			return m[0][0] + m[1][1];
		}
	};

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
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<ImplTraits_T<M>>)
				return m[0].get<0>() + m[1].get<1>() + m[2].get<2>() + m[3].get<3>();
			else
#endif // UBPA_UGM_USE_SIMD
				return m[0][0] + m[1][1] + m[2][2] + m[3][3];
		}
	};

	// =======================================================

	template<size_t N>
	struct init;

	template<>
	struct init<2> {
		template<typename M>
		inline static void run(M& m, const std::array<ImplTraits_F<M>, 2 * 2>& data) noexcept {
			static_assert(M::N == 2);
			memcpy(&m, data.data(), 4 * sizeof(ImplTraits_F<M>));
		}
	};

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
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<ImplTraits_T<M>>) {
				m[0] = _mm_loadu_ps(&(data[0]));
				m[1] = _mm_loadu_ps(&(data[4]));
				m[2] = _mm_loadu_ps(&(data[8]));
				m[3] = _mm_loadu_ps(&(data[12]));
			}
			else
#endif // UBPA_UGM_USE_SIMD
			{
				memcpy(&m, data.data(), 16 * sizeof(ImplTraits_F<M>));
			}
		}
	};

	// =======================================================

	template<size_t N>
	struct zero;

	template<>
	struct zero<2> {
		template<typename M>
		inline static M run() noexcept {
			static_assert(M::N == 2);
			return {
				0,0,
				0,0,
			};
		}
	};

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
#ifdef UBPA_UGM_USE_SIMD
			if constexpr (ImplTraits_SupportSIMD<ImplTraits_T<M>>) {
				using V = ImplTraits_T<M>;
				const __m128 z = _mm_set1_ps(0.f);
				return { V{z}, V{z}, V{z}, V{z} };
			}
			else
#endif // UBPA_UGM_USE_SIMD
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

	// =========================

	template<size_t N>
	struct SVD;

	template<>
	struct SVD<2> {
		template<typename M>
		inline static std::tuple<M, M, M> run(const M& m) noexcept {
			static_assert(M::N == 2);
			using F =ImplTraits_F<M>;
			// ref : https://lucidar.me/en/mathematics/singular-value-decomposition-of-a-2x2-matrix/
			F a = m(0, 0);
			F b = m(0, 1);
			F c = m(1, 0);
			F d = m(1, 1);
			F a2 = pow2(a);
			F b2 = pow2(b);
			F c2 = pow2(c);
			F d2 = pow2(d);
			F a2_p_b2 = a2 + b2;
			F c2_p_d2 = c2 + d2;
			F ac_p_bd = a * c + b * d;
			F two_ac_p_bd = 2 * ac_p_bd;
			F a2_p_b2_m_c2_m_d2 = a2_p_b2 - c2_p_d2;

			F theta = static_cast<F>(0.5) * std::atan2(two_ac_p_bd        , a2_p_b2_m_c2_m_d2);
			F phi   = static_cast<F>(0.5) * std::atan2(2 * (a * b + c * d), a2 - b2 + c2 - d2);

			F cos_theta = std::cos(theta);
			F sin_theta = std::sin(theta);
			F cos_phi   = std::cos(phi);
			F sin_phi   = std::sin(phi);

			M U{
				cos_theta, -sin_theta,
				sin_theta,  cos_theta,
			};

			F S1 = a2_p_b2 + c2_p_d2;
			F S2 = std::sqrt(pow2(a2_p_b2_m_c2_m_d2) + pow2(two_ac_p_bd));
			F sigma1 = std::sqrt(static_cast<F>(0.5) * (S1 + S2));
			F sigma2 = std::sqrt(static_cast<F>(0.5) * (S1 - S2));

			M Sigma{
				sigma1,      0,
				     0, sigma2,
			};

			F s11 = (a * cos_theta + c * sin_theta) * cos_theta + ( b * cos_theta + d * sin_theta) * sin_phi;
			F s22 = (a * sin_theta - c * cos_theta) * sin_phi   + (-b * sin_theta + d * cos_theta) * cos_phi;
			F sign_s11 = static_cast<F>(s11 > 0 ? 1 : (s11 < 0 ? -1 : 0));
			F sign_s22 = static_cast<F>(s22 > 0 ? 1 : (s22 < 0 ? -1 : 0));

			M V{
				sign_s11 * cos_phi, -sign_s22 * sin_phi,
				sign_s11 * sin_phi,  sign_s22 * cos_phi,
			};

			return { U, Sigma, V };
		}
	};

	template<>
	struct SVD<3> {
		template<typename M>
		inline static std::tuple<M, M, M> run(const M& m) noexcept {
			static_assert(M::N == 3 && std::is_same_v<ImplTraits_F<M>, float>);
			// ref : https://github.com/ericjang/svd3

			M U, S, V;
			EricJang::svd(
				// M
				m(0, 0), m(0, 1), m(0, 2),
				m(1, 0), m(1, 1), m(1, 2),
				m(2, 0), m(2, 1), m(2, 2),
				// U
				U(0, 0), U(0, 1), U(0, 2),
				U(1, 0), U(1, 1), U(1, 2),
				U(2, 0), U(2, 1), U(2, 2),
				// S
				S(0, 0), S(0, 1), S(0, 2),
				S(1, 0), S(1, 1), S(1, 2),
				S(2, 0), S(2, 1), S(2, 2),
				// V
				V(0, 0), V(0, 1), V(0, 2),
				V(1, 0), V(1, 1), V(1, 2),
				V(2, 0), V(2, 1), V(2, 2)
			);

			return { U, S, V };
		}
	};
}
