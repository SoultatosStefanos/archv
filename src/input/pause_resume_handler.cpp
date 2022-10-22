#include "pause_resume_handler.hpp"

#include <boost/log/trivial.hpp>
#include <cassert>

namespace input
{

pause_resume_handler::pause_resume_handler(
    application_type& app,
    cameraman_type& cameraman,
    gui_handler_type& gui_handler)
: m_app { app }, m_cameraman { cameraman }, m_gui_handler { gui_handler }
{
    assert(!paused());
}

auto pause_resume_handler::keyReleased(const OgreBites::KeyboardEvent& e)
    -> bool
{
    if (e.keysym.sym == 'p')
        toggle_pause_resume();
    return true;
}

auto pause_resume_handler::toggle_pause_resume() -> void
{
    if (!paused())
        pause();
    else
        resume();
}

auto pause_resume_handler::pause() -> void
{
    assert(!paused());

    cameraman().manualStop();
    app().removeInputListener(&cameraman());
    app().addInputListener(&gui_handler());

    m_paused = true;

    BOOST_LOG_TRIVIAL(info) << "paused";

    assert(paused());
}

auto pause_resume_handler::resume() -> void
{
    assert(paused());

    app().removeInputListener(&gui_handler());
    app().addInputListener(&cameraman());

    m_paused = false;

    BOOST_LOG_TRIVIAL(info) << "resumed";

    assert(!paused());
}

} // namespace input