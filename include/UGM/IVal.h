#pragma once
#ifndef UBPA_GM_IVAL_H
#define UBPA_GM_IVAL_H

#include <array>
#include <iostream>

#include <assert.h>

namespace Ubpa {
	template<typename Base, typename Impl, typename T, typename N>
	struct IVal : Base, std::array<T, N::value> {
		using Base::Base;

		IVal() { }
		template<typename... U>
		IVal(U... data) : std::array<T, N::value>{static_cast<T>(data)...} {
			static_assert(sizeof...(U) == N::value, "sizeof...(U) == N::value");
		}

		friend std::ostream& operator<<(std::ostream& os, const Impl& x) {
			// for-loop will be optimized in -02 (release)
			for (auto i = static_cast<typename N::type>(0); i < N::value - 1; i++)
				os << x[i] << ",";
			os << x[N::value - 1];
			return os;
		}

		friend std::istream& operator>>(std::istream& is, Impl& x) {
			// for-loop will be optimized in -02 (release)
			for (auto i = static_cast<typename N::type>(0); i < N::value; i++)
				is >> x[i];
			return is;
		}
	};
}

#endif // !UBPA_GM_IVAL_H
