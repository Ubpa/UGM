#pragma once

#include "forward_decl.h"
#include <UTemplate/SI.h>

namespace Ubpa {
	template<typename T_, size_t N_>
	struct ArrayTraits {
		using T = T_;
		static constexpr size_t N = N_;
	};
}
