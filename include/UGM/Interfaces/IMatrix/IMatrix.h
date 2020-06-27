#pragma once

#include "../IArray/IArrayUtil.h"

#include "IMatrix_detail.h"

namespace Ubpa {
	// simple [square] 2D array
	// column first
	template<typename Base, typename Impl>
	struct IMatrix : Base {
		using Base::Base;

		using Vector = ImplTraits_T<Impl>;

		static_assert(Vector::template IsContain<IArray>());

		using F = ImplTraits_F<Impl>;
		static constexpr size_t N = ImplTraits_N<Impl>;

		static_assert(N >= 2 && N <= 4);
		static_assert(Vector::N == N);

		// column first
		inline IMatrix(const std::array<F, N * N>& data) noexcept { init(data); }

		// column first
		inline void init(const std::array<F, N* N>& data) noexcept {
			auto& m = static_cast<Impl&>(*this);
			detail::IMatrix_::init<N>::run(m, data);
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
		inline void init(Us... vals) noexcept {
			auto t = std::make_tuple(static_cast<F>(vals)...);
			if constexpr (N == 2) {
				init(std::array<F, 2 * 2>{
					std::get<0>(t), std::get<2>(t),
					std::get<1>(t), std::get<3>(t),
				});
			}else if constexpr (N == 3) {
				init(std::array<F, 3 * 3>{
					std::get<0>(t), std::get<3>(t), std::get<6>(t),
					std::get<1>(t), std::get<4>(t), std::get<7>(t),
					std::get<2>(t), std::get<5>(t), std::get<8>(t),
				});
			}
			else // if constexpr (N == 4)
			{
				init(std::array<F, 4 * 4>{
					std::get< 0>(t), std::get< 4>(t), std::get< 8>(t), std::get<12>(t),
					std::get< 1>(t), std::get< 5>(t), std::get< 9>(t), std::get<13>(t),
					std::get< 2>(t), std::get< 6>(t), std::get<10>(t), std::get<14>(t),
					std::get< 3>(t), std::get< 7>(t), std::get<11>(t), std::get<15>(t),
				});
			}
		}

		inline static const Impl eye() noexcept {
			return detail::IMatrix_::eye<Impl, N>::run();
		}

		inline static const Impl zero() noexcept {
			return detail::IMatrix_::zero<N>::template run<Impl>();
		}

		inline F& operator()(size_t r, size_t c) noexcept {
			assert(r < N && c < N);
			return (*this)[c][r];
		}

		inline F operator()(size_t r, size_t c) const noexcept {
			assert(r < N && c < N);
			return (*this)[c][r];
		}

		inline F& at(size_t r, size_t c) noexcept {
			return this->operator()(r, c);
		}

		inline F at(size_t r, size_t c) const noexcept {
			return this->operator()(r, c);
		}

		inline F& operator()(size_t n) noexcept {
			assert(n < N * N);
			return (*this)[n % N][n / N];
		}

		inline F operator()(size_t n) const noexcept {
			assert(n < N * N);
			return (*this)[n % N][n / N];
		}

		inline F& at(size_t n) noexcept {
			assert(n < N * N);
			return (*this)[n % N][n / N];
		}

		inline F at(size_t n) const noexcept {
			assert(n < N * N);
			return (*this)[n % N][n / N];
		}

		inline F trace() const noexcept {
			const auto& m = static_cast<const Impl&>(*this);
			return detail::IMatrix_::trace<N>::run(m);
		}

		inline const Impl transpose() const noexcept {
			const auto& m = static_cast<const Impl&>(*this);
			return detail::IMatrix_::transpose<N>::run(m);
		}

		// not accurate in 3x3 but fast (1~2 ms)
		inline const std::tuple<Impl, Impl, Impl> SVD() const noexcept {
			const auto& m = static_cast<const Impl&>(*this);
			static_assert(N == 2 || (N == 3 && std::is_same_v<F, float>)); // only support 2x2 and 3x3 matrix by now
			return detail::IMatrix_::SVD<N>::run(m);
		}

		F* data() noexcept {
			return reinterpret_cast<F*>(this);
		}

		const F* data() const noexcept {
			return const_cast<IMatrix*>(this)->data();
		}
	};

	InterfaceTraits_Regist(IMatrix,
		IArrayUtil);
}
