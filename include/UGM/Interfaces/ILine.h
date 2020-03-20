#pragma once

#include "ILinear.h"
#include "IAffine.h"

#include <tuple>

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct ILine : std::tuple<Arg_ImplP<ArgList>, Arg_ImplV<ArgList>>,
		SIVT_CRTP<TemplateList<IAffine>, Base, Impl, ArgList>
	{
		using ImplP = Arg_ImplP<ArgList>;
		using ImplV = Arg_ImplV<ArgList>;
		using F = Arg_F<ArgList>;

		static_assert(ExistInstance_v<typename ImplP::AllVBs, IAffine>);
		static_assert(ExistInstance_v<typename ImplV::AllVBs, ILinear>);

		using SIVT_CRTP<TemplateList<IAffine>, Base, Impl, ArgList>::SIVT_CRTP;
		using std::tuple<Arg_ImplP<ArgList>, Arg_ImplV<ArgList>>::tuple;

		ImplP& origin() noexcept { return std::get<ImplP>(*this); }
		const ImplP& origin() const noexcept { return std::get<ImplP>(*this); }
		ImplV& dir() noexcept { return std::get<ImplV>(*this); }
		const ImplV& dir() const noexcept { return std::get<ImplV>(*this); }

		const ImplP at(F t) const noexcept { return origin() + t * dir(); }
	};
}
