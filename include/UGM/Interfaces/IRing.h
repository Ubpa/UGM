#pragma once

#include "IAdd.h"
#include "IMul.h"

#include <UTemplate/SI.h>

namespace Ubpa {
	CombineInterface(IRing, IAdd, IMul);
}
