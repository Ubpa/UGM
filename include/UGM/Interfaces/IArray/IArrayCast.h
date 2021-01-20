#pragma once

#include "IArray.h"
#include "IArray1D.h"

namespace Ubpa {
	template<typename Base, typename Impl>
	struct IArrayCast : Base {
		using Base::Base;

		static constexpr size_t N = SI_ImplTraits_N<Impl>;

		template<typename To>
		To cast_to() const noexcept {
			constexpr size_t M = To::N;
			static_assert(M <= N && SI_Contains_v<Impl, IArray1D> == SI_Contains_v<To, IArray1D>);
			return cast_to<To>(std::make_index_sequence<M>{});
		}

		// reinterpret cast
		template<typename To>
		To& as() & noexcept {
			static_assert(sizeof(To) == sizeof(Impl)
				// && std::is_same_v<SI_ImplTraits_T<To>, SI_ImplTraits_T<Impl>>
				&& alignof(Impl) >= alignof(To));
			return reinterpret_cast<To&>(*this);
		}

		template<typename To>
		const To& as() const& noexcept {
			return const_cast<IArrayCast*>(this)->as<To>();
		}

	private:
		template<typename To, size_t... Ns>
		To cast_to(std::index_sequence<Ns...>) const noexcept {
			return { static_cast<typename To::T>((*this)[Ns])... };
		}
	};
}

SI_InterfaceTraits_Register(Ubpa::IArrayCast,
	Ubpa::IArray
);
