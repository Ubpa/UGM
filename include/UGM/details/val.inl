#pragma once

namespace Ubpa {
	template<typename T, size_t N>
	template<size_t M>
	val<T, N>::operator mat<T, M>() noexcept {
		static_assert(M * M == N);
		mat<T, M> rst;
		for(size_t j = 0; j < M; j++) {
			for(size_t i = 0; i < M; i++) {
			    rst[j][i] = (*this)[j*M + i];
			}
		}
		return rst;
	}

	template<typename T, size_t N>
	val<T, N>::operator transform<T>() noexcept {
		static_assert(N == 16);
		transform<T> rst;
		rst[0][0] = (*this)[ 0];
		rst[0][1] = (*this)[ 1];
		rst[0][2] = (*this)[ 2];
		rst[0][3] = (*this)[ 3];
		rst[1][0] = (*this)[ 4];
		rst[1][1] = (*this)[ 5];
		rst[1][2] = (*this)[ 6];
		rst[1][3] = (*this)[ 7];
		rst[2][0] = (*this)[ 8];
		rst[2][1] = (*this)[ 9];
		rst[2][2] = (*this)[10];
		rst[2][3] = (*this)[11];
		rst[3][0] = (*this)[12];
		rst[3][1] = (*this)[13];
		rst[3][2] = (*this)[14];
		rst[3][3] = (*this)[15];
		return rst;
	}
}
