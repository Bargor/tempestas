#pragma once

#include "assert.h"
#include "platform.h"

#include <memory>

template<typename T, typename Deleter = std::default_delete<T>>
using ptr = std::unique_ptr<T, Deleter>;
