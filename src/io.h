#pragma once

#include "buffer.h"

void io_loop(caf::scoped_actor& self, buffer_actor& buffer);
