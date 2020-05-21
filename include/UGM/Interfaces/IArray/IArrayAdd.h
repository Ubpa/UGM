#pragma once

#include "IArray.h"
#include "../IAdd.h"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IArrayAdd : Base {
		using Base::Base;

		using T = ImplTraits_T<Impl>;
		static constexpr size_t N = ImplTraits_N<Impl>;


	private:
		template<typename Base, typename Impl>
		friend struct IAdd;

		inline const Impl impl_add(const Impl& y) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4)
				return x.get() + y.get();
			else
#endif // UBPA_USE_XSIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = x[i] + y[i];
				return rst;
			}
		}

		inline Impl& impl_add_to_self(const Impl& y) noexcept {
			auto& x = static_cast<Impl&>(*this);
#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4)
				return x += y;
			else
#endif // UBPA_USE_XSIMD
			{
				for (size_t i = 0; i < N; i++)
					x[i] += y[i];
				return x;
			}
		}

		inline const Impl impl_add_inverse() const noexcept {
			auto& x = static_cast<const Impl&>(*this);
#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4)
				return -x;
			else
#endif // UBPA_USE_XSIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = -x[i];
				return rst;
			}
		}

		inline const Impl impl_minus(const Impl& y) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4) {
				return x - y;
			}
			else
#endif // UBPA_USE_XSIMD
			{
				Impl rst;
				for (size_t i = 0; i < N; i++)
					rst[i] = x[i] - y[i];
				return rst;
			}
		}

		inline Impl& impl_minus_to_self(const Impl& y) noexcept {
			auto& x = static_cast<Impl&>(*this);
#ifdef UBPA_USE_XSIMD
			if constexpr (std::is_same_v<T, float> && N == 4) {
				return x -= y;
			}
			else
#endif // UBPA_USE_XSIMD
			{
				for (size_t i = 0; i < N; i++)
					x[i] -= y[i];
				return x;
			}
		}
	};

	InterfaceTraits_Regist(IArrayAdd,
		IAdd, IArray);
}
