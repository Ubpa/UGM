#pragma once

#include "IAdd.h"
#include "IMul.h"

#include <UTemplate/SI.h>

namespace Ubpa {
	template<typename Base, typename Impl> struct IRing;
}

SI_CombineInterface(Ubpa::IRing,
	Ubpa::IAdd,
	Ubpa::IMul
);
