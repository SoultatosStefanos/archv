#include "camera_handler.hpp"

#include <boost/log/trivial.hpp>

namespace input
{

camera_handler::camera_handler(cameraman_type cameraman)
: m_cameraman { std::move(cameraman) }
{
}

auto camera_handler::lock() -> void
{
    cameraman().manualStop();
    m_locked = true;
    BOOST_LOG_TRIVIAL(info) << "camera locked";
}

auto camera_handler::unlock() -> void
{
    m_locked = false;
    BOOST_LOG_TRIVIAL(info) << "camera unlocked";
}

auto camera_handler::toggle_lock_unlock() -> void
{
    if (locked())
        unlock();
    else
        lock();
}

auto camera_handler::frameRendered(const frame_event_type& evt) -> void
{
    if (!locked())
        cameraman().frameRendered(evt);
}

auto camera_handler::keyPressed(const keyboard_event_type& evt) -> bool
{
    if (!locked())
        return cameraman().keyPressed(evt);

    return true;
}

auto camera_handler::keyReleased(const keyboard_event_type& evt) -> bool
{
    if (evt.keysym.sym == 'c')
        toggle_lock_unlock();

    if (!locked())
        return cameraman().keyReleased(evt);

    return true;
}

auto camera_handler::mouseMoved(const mouse_motion_event_type& evt) -> bool
{
    if (!locked())
        return cameraman().mouseMoved(evt);

    return true;
}

auto camera_handler::mousePressed(const mouse_button_event_type& evt) -> bool
{
    if (evt.button == OgreBites::ButtonType::BUTTON_MIDDLE)
        unlock();

    return true;
}

auto camera_handler::mouseReleased(const mouse_button_event_type& evt) -> bool
{
    if (evt.button == OgreBites::ButtonType::BUTTON_MIDDLE)
        lock();

    return true;
}

} // namespace input