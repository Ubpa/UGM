#pragma once

#include "details/traits.h"

#include "Interfaces/ILinear.h"
#include "Interfaces/IArray/IArrayLinear.h"
#include "Interfaces/IArray/IArrayHadamardProduct.h"
#include "Interfaces/IArray/IArray1D_Util.h"

namespace Ubpa {
	template<typename T>
	struct ImplTraits<rgb<T>>
		: Array1DTraits<T, 3>,
		IListTraits<IArray1D_Util, IArrayHadamardProduct, IArrayLinear> {};

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
	};

	using rgbf = rgb<float>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(rgbf) == 3 * sizeof(float));
}
