#pragma once

#include "arg_parser.h"
#include "main_window.h"

namespace tst::device {
class monitor;
}

namespace tst::application {

class application final {
public:
    application(program_params params, const device::monitor& monitor) noexcept;

    void run() noexcept;

private:
    program_params m_program_params;
    main_window m_main_window;
};

} // namespace tst::application
