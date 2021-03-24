#pragma once

#include "IAdd.hpp"
#include "IMul.hpp"

#include <UTemplate/SI.hpp>

namespace Ubpa {
	template<typename Base, typename Impl> struct IRing;
}

SI_CombineInterface(Ubpa::IRing,
	Ubpa::IAdd,
	Ubpa::IMul
);
