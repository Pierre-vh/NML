#pragma once

#include "../Errors/error.h"
#include <memory>

namespace Easy
{
	extern std::unique_ptr<Easy::error> reporter;
}
