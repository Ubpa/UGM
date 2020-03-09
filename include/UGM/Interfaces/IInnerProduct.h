#pragma once

#include "INorm.h"

namespace Ubpa {
	template<typename Base, typename Impl, typename ArgList>
	struct IInnerProduct : SIVT_CRTP<TemplateList<INorm>, Base, Impl, ArgList> {
		using F = Arg_F<ArgList>;

		using SIVT_CRTP<TemplateList<INorm>, Base, Impl, ArgList>::SIVT_CRTP;

		inline static F dot(const Impl& x, const Impl& y) noexcept {
			return Impl::impl_dot(x, y);
		}

		inline F dot(const Impl& y) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			return dot(x, y);
		}

		inline F norm2() const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			return dot(x, x);
		}

		inline static F distance2(const Impl& x, const Impl& y) noexcept {
			return (x - y).norm2();
		}

		inline F distance2(const Impl& y) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			return distance2(x, y);
		}

		inline static F cos_theta(const Impl& x, const Impl& y) noexcept {
			F xN = x.norm();
			F yN = y.norm();
			F xyN = xN * yN;
			assert(xyN != static_cast<F>(0));
			return Impl::dot(x, y) / xyN;
		}

		inline F cos_theta(const Impl& y) const noexcept {
			auto& x = static_cast<const Impl&>(*this);
			return cos_theta(x, y);
		}

	private:
		template<typename Base, typename Impl, typename ArgList>
		friend struct INorm;

		inline F impl_norm() const noexcept {
			return std::sqrt(norm2());
		}
	};
}