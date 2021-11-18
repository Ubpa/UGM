#pragma once

#include "../IArray/IArrayUtil.hpp"

#include "details/IMatrix.inl"

namespace Ubpa {
	// simple [square] 2D array
	// column first
	template<typename Base, typename Impl>
	struct IMatrix : Base {
		using Base::Base;

		using Vector = SI_ImplTraits_T<Impl>;

		static_assert(SI_Contains_v<Vector, IArray>);

		using F = SI_ImplTraits_F<Impl>;
		static constexpr size_t N = SI_ImplTraits_N<Impl>;

		static_assert(N >= 2 && N <= 4);
		static_assert(Vector::N == N);

		// column first
		IMatrix(const std::array<F, N * N>& data) noexcept { init(data); }

		// column first
		void init(const std::array<F, N* N>& data) noexcept {
			auto& m = static_cast<Impl&>(*this);
			details::IMatrix_::init<N>::run(m, data);
		}

		// row first
		// -o-o-o-o-o-o-
		// U0 is used to avoid MSVC's compiler bug
		// because IArrayImpl has same constructor:
		// - template<typename... Us, nullptr> IArrayImpl(Us... vals);
		template<typename U0, typename... Us, std::enable_if_t<1 + sizeof...(Us) == N * N>* = nullptr>
		constexpr IMatrix(U0 val0, Us... vals) noexcept { init(val0, vals...); }

		// row first
		template<typename... Us, std::enable_if_t<sizeof...(Us) == N * N>* = nullptr>
		void init(Us... vals) noexcept {
			F t[] {static_cast<F>(vals)...};
			if constexpr (N == 2) {
				init(std::array<F, 2 * 2>{
					t[0], t[2],
					t[1], t[3],
				});
			}else if constexpr (N == 3) {
				init(std::array<F, 3 * 3>{
					t[0], t[3], t[6],
					t[1], t[4], t[7],
					t[2], t[5], t[8],
				});
			}
			else // if constexpr (N == 4)
			{
				init(std::array<F, 4 * 4>{
					t[0], t[4], t[ 8], t[12],
					t[1], t[5], t[ 9], t[13],
					t[2], t[6], t[10], t[14],
					t[3], t[7], t[11], t[15],
				});
			}
		}

		static Impl eye() noexcept {
			return details::IMatrix_::eye<Impl, N>::run();
		}

		static Impl Zero() noexcept {
			return details::IMatrix_::Zero<N>::template run<Impl>();
		}

		F& operator()(size_t r, size_t c) noexcept {
			assert(r < N && c < N);
			return (*this)[c][r];
		}

		F operator()(size_t r, size_t c) const noexcept {
			assert(r < N && c < N);
			return (*this)[c][r];
		}

		F& at(size_t r, size_t c) noexcept {
			return this->operator()(r, c);
		}

		F at(size_t r, size_t c) const noexcept {
			return this->operator()(r, c);
		}

		F& operator()(size_t n) noexcept {
			assert(n < N * N);
			return (*this)[n % N][n / N];
		}

		F operator()(size_t n) const noexcept {
			assert(n < N * N);
			return (*this)[n % N][n / N];
		}

		F& at(size_t n) noexcept {
			assert(n < N * N);
			return (*this)[n % N][n / N];
		}

		F at(size_t n) const noexcept {
			assert(n < N * N);
			return (*this)[n % N][n / N];
		}

		F trace() const noexcept {
			const auto& m = static_cast<const Impl&>(*this);
			return details::IMatrix_::trace<N>::run(m);
		}

		Impl transpose() const noexcept {
			const auto& m = static_cast<const Impl&>(*this);
			return details::IMatrix_::transpose<N>::run(m);
		}

		// return (U, S, V)
		// not accurate in 3x3 but fast (1~2 ms)
		std::tuple<Impl, Impl, Impl> SVD() const noexcept {
			const auto& m = static_cast<const Impl&>(*this);
			static_assert(N == 2 || (N == 3 && std::is_same_v<F, float>)); // only support 2x2 and 3x3 matrix by now
			return details::IMatrix_::SVD<N>::run(m);
		}

		// return (U, S, V)
		// det(UV^T) > 0, last sigma may be negative
		// not accurate in 3x3 but fast (1~2 ms)
		std::tuple<Impl, Impl, Impl> signed_SVD() const noexcept {
			auto [U, S, V] = SVD();
			auto U_VT = U * V.transpose();
			if (U_VT.det() < 0) {
				V[N - 1] = -V[N - 1];
				S[N - 1] = -S[N - 1];
			}
			return { U,S,V };
		}

		F det() const noexcept {
			const auto& m = static_cast<const Impl&>(*this);
			return details::IMatrix_::det<N>::run(m);
		}

		F* data() noexcept {
			return reinterpret_cast<F*>(this);
		}

		const F* data() const noexcept {
			return const_cast<IMatrix*>(this)->data();
		}
	};
}

SI_InterfaceTraits_Register(Ubpa::IMatrix,
	Ubpa::IArrayUtil
);
