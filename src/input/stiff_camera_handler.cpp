#include "stiff_camera_handler.hpp"

namespace input
{

stiff_camera_handler::stiff_camera_handler(cameraman_type cameraman)
: base(std::move(cameraman))
{
}

auto stiff_camera_handler::mouseMoved(const OgreBites::MouseMotionEvent& evt)
    -> bool
{
    return InputListener::mouseMoved(evt);
}

auto stiff_camera_handler::mousePressed(const OgreBites::MouseButtonEvent& evt)
    -> bool
{
    return InputListener::mousePressed(evt);
}

auto stiff_camera_handler::mouseReleased(const OgreBites::MouseButtonEvent& evt)
    -> bool
{
    return InputListener::mouseReleased(evt);
}

} // namespace input