#pragma once

#include "rgb.hpp"

#include "Interfaces/IArray/IArray1D_Util.hpp"

namespace Ubpa {
	template<typename T>
	struct SI_ImplTraits<rgba<T>>
		: Array1DTraits<T, 4>,
		SIMDTraits<false>, // float4 not use SIMD
		IListTraits<IArray1D_Util>,
		ArrayImplNTraits<val, T> {};

	template<typename T>
	struct rgba : SI<rgba<T>> {
		using Base = SI<rgba<T>>;
		using Base::Base;

		rgba(const rgb<T>& rgb, T a) noexcept
			: Base{ rgb[0], rgb[1], rgb[2], a } {}
		explicit rgba(const rgb<T>& rgb) noexcept : rgba(rgb, One<T>) {}

	public:
		rgb<T> to_rgb() const noexcept {
			return rgb<T>((*this)[0], (*this)[1], (*this)[2]) * (*this)[3];
		}

		rgb<T> over(const rgb<T>& rgb) const noexcept {
			return to_rgb() + rgb * (One<T> - (*this)[3]);
		}

		rgba over(const rgba& c) const noexcept {
			// rstA = 1 - (1-a)(1-c[3])
			T a = (*this)[3];
			const auto rstA = a + c[3] - a * c[3];
			if (rstA == Zero<T>)
				return rgba(0, 0, 0, 0);

			const auto rstRGB = (to_rgb() + c.to_rgb() * a) / rstA;
			return rgba(rstRGB, rstA);
		}
	};

	using rgbaf = rgba<float>;
	using rgbad = rgba<double>;

	// maybe error in editor, but no compile error
	static_assert(sizeof(rgbaf) == 4 * sizeof(float));
}
