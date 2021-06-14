#pragma once

#include "val.hpp"

#include "details/traits.hpp"

#include "Interfaces/ILinear.hpp"
#include "Interfaces/IArray/IArrayLinear.hpp"
#include "Interfaces/IArray/IArrayHadamardProduct.hpp"
#include "Interfaces/IArray/IArray1D_Util.hpp"

namespace Ubpa {
	template<typename T>
	struct SI_ImplTraits<rgb<T>>
		: Array1DTraits<T, 3>,
		IListTraits<IArray1D_Util, IArrayHadamardProduct, IArrayLinear>,
		ArrayImplNTraits<val, T> {};

	template<typename T>
	struct rgb : SI<rgb<T>> {
		using SI<rgb<T>>::SI;

		T illumination() const noexcept {
			return static_cast<T>(0.2126)* (*this)[0]
				+ static_cast<T>(0.7152)* (*this)[1]
				+ static_cast<T>(0.0722)* (*this)[2];
		}

		T gray() const noexcept {
			return
				(*this)[0] * static_cast<T>(0.299)
				+ (*this)[1] * static_cast<T>(0.587)
				+ (*this)[2] * static_cast<T>(0.114);
		}

		rgb gamma_to_linear() const noexcept {
			return {
				Ubpa::gamma_to_linear((*this)[0]),
				Ubpa::gamma_to_linear((*this)[1]),
				Ubpa::gamma_to_linear((*this)[2])
			};
		}

		rgb linear_to_gamma() const noexcept {
			return {
				Ubpa::linear_to_gamma((*this)[0]),
				Ubpa::linear_to_gamma((*this)[1]),
				Ubpa::linear_to_gamma((*this)[2])
			};
		}
	};

	using rgbf = rgb<float>;
	using rgbd = rgb<double>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(rgbf) == 3 * sizeof(float));
}
