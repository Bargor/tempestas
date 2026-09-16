#include <core/util/variant.h>
#include <variant>

namespace tst::core {

using test_variant = std::variant<int, float, bool>;

static_assert(variant_index<int, test_variant>() == 0);
static_assert(variant_index<float, test_variant>() == 1);
static_assert(variant_index<bool, test_variant>() == 2);

} // namespace tst::core
