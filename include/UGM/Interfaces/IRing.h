#pragma once

#include "IAdd.h"
#include "IMul.h"

#include <UTemplate/SI.h>

namespace Ubpa {
	SI_CombineInterface(IRing,
		IAdd,
		IMul
	);
}
