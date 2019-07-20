#pragma once

#include <AK/RefPtr.h>

#include "Program.h"

RefPtr<Program> parse(const String&);
