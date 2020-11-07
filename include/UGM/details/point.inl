#pragma once

namespace Ubpa {
	template<typename T, size_t N>
	template<typename Container>
	point<T, N> point<T, N>::combine(const Container& points, T weight) noexcept {
		point rst{ ZERO<T> };
		for (const auto& p : points) {
			for (size_t i = 0; i < N; i++)
				rst[i] += weight * p[i];
		}
		return rst;
	}

	template<typename T, size_t N>
	template<typename PContainer, typename WContainer>
	point<T, N> point<T, N>::combine(PContainer points, WContainer weights) noexcept {
		assert(points.size() == weights.size());
		point rst{ ZERO<T> };
		auto witer = weights.begin();
		for (const auto& p : points) {
			T weight = *witer;
			for (size_t i = 0; i < N; i++)
				rst[i] += weight * p[i];
			++witer;
		}
		return rst;
	}
}
