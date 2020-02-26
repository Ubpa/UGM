#pragma once

#include "euler.h"

#include "Interfaces/IQuat/IQuatMul.h"
#include "Interfaces/IQuat/IQuatInOut.h"
#include "point.h"
#include "vec.h"

namespace Ubpa {
	template<typename T>
	struct euler;

	template<typename T>
	struct quat : SIIT_CRTP<TemplateList<IQuatInOut, IQuatMul>, quat<T>, T> {
		using SIIT_CRTP<TemplateList<IQuatInOut, IQuatMul>, quat<T>, T>::SIIT_CRTP;

		euler<T> to_euler() const noexcept {
			T x = this->imag()[0];
			T y = this->imag()[1];
			T z = this->imag()[2];
			T w = this->real();

			T sX = 2 * (x * w - y * z);
			if (std::abs(sX) == static_cast<T>(1)) {
				T half_sY = x * y - z * w;
				T half_cY = y * z + x * w;
				return { std::asin(sX), std::atan2(half_sY, half_cY), 0 };
			}
			else {
				T cXsY = 2 * (x * z + y * w);
				T cXcY = 1 - 2 * (x * x + y * y);
				T cXsZ = 2 * (x * y + z * w);
				T cXcZ = 1 - 2 * (z * z + x * x);
				return{ std::asin(sX), std::atan2(cXsY,cXcY), std::atan2(cXsZ,cXcZ) };
			}
		}
	};

	using quatf = quat<float>;
}
