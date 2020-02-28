#pragma once

#include "../IArray/IArray.h"

#include "IMatrix_detail.h"

#include <array>

namespace Ubpa {
	// simple [square] 2D array
	// column first
	template<typename Base, typename Impl, typename ArgList>
	struct IMatrix : SIVT_CRTP<TemplateList<IArray>, Base, Impl, ArgList> {
		static_assert(ExistInstance_v<typename Arg_T<ArgList>::AllVBs, IArray>);

		using F = Arg_F<ArgList>;
		static constexpr size_t N = Arg_N<ArgList>;

		static_assert(N == 3 || N == 4);
		static_assert(Arg_T<ArgList>::N == N);

		using SIVT_CRTP<TemplateList<IArray>, Base, Impl, ArgList>::SIVT_CRTP;

		IMatrix(const std::array<F,N*N>& data) noexcept { init(data); }

		void init(const std::array<F, N * N>& data) noexcept {
			// unloop in /O2
			for (size_t i = 0; i < N * N; i++)
				(*this)(i) = data[i];
		}

		static const Impl eye() noexcept {
			return detail::eye<Impl, N>::run();
		}

		static const Impl zero() noexcept {
			Impl rst{};
			for (size_t i = 0; i < N * N; i++)
				(*this)(i) = static_cast<F>(0);
			return rst;
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
			F rst = (*this)[0][0];
			for (size_t i = 1; i < N; i++)
				rst += (*this)[i][i];
			return rst;
		}

		const Impl transpose() const noexcept {
			const auto& m = static_cast<const Impl&>(*this);
			return detail::transpose<N>::run(m);
		}
	};
}
