#pragma once

#include "IAffineSubspace.h"

namespace Ubpa {
	template<typename Base, typename ImplP, typename ArgList>
	struct IAffine : SIVT_CRTP<TemplateList<IAffineSubspace>, Base, ImplP, ArgList> {
		using ImplV = Arg_ImplV<ArgList>;
		
		static_assert(ExistInstance_v<typename ImplV::AllVBs, ILinear>);

		using DBase = SIVT_CRTP<TemplateList<IAffineSubspace>, Base, ImplP, ArgList>;
		using DBase::DBase;
		using DBase::operator-;

		inline const ImplV operator-(const ImplP& y) const noexcept {
			return static_cast<const ImplP*>(this)->impl_affine_minus(y);
		}
	};
}
