#pragma once

#include "arg_parser.h"
#include "input_processor.h"
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
    event_processor<event> m_events_processor;
    main_window m_main_window;
    input_processor m_input_processor;
};

} // namespace tst::application
