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

auto camera_handler::frameRendered(const Ogre::FrameEvent& evt) -> void
{
    if (!locked())
        cameraman().frameRendered(evt);
}

auto camera_handler::keyPressed(const OgreBites::KeyboardEvent& evt) -> bool
{
    if (!locked())
        cameraman().keyPressed(evt);

    return base::keyPressed(evt);
}

auto camera_handler::keyReleased(const OgreBites::KeyboardEvent& evt) -> bool
{
    if (evt.keysym.sym == 'c')
        toggle_lock_unlock();

    if (!locked())
        cameraman().keyReleased(evt);

    return base::keyReleased(evt);
}

auto camera_handler::mouseMoved(const OgreBites::MouseMotionEvent& evt) -> bool
{
    if (!locked())
        cameraman().mouseMoved(evt);

    return base::mouseMoved(evt);
}

auto camera_handler::mousePressed(const OgreBites::MouseButtonEvent& evt)
    -> bool
{
    if (evt.button == OgreBites::ButtonType::BUTTON_MIDDLE)
        unlock();

    return base::mousePressed(evt);
}

auto camera_handler::mouseReleased(const OgreBites::MouseButtonEvent& evt)
    -> bool
{
    if (evt.button == OgreBites::ButtonType::BUTTON_MIDDLE)
        lock();

    return base::mouseReleased(evt);
}

} // namespace input