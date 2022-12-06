// Contains a camera handler variant, which can only move, not look around.
// Soultatos Stefanos 2022

#ifndef INPUT_STIFF_CAMERA_HANDLER_HPP
#define INPUT_STIFF_CAMERA_HANDLER_HPP

#include "camera_handler.hpp" // for camera_handler

namespace input
{

/***********************************************************
 * Stiff Camera Handler                                    *
 ***********************************************************/

// Moves a camera, but doesn't look around with the mouse. So stiff.
class stiff_camera_handler : public camera_handler
{
    using base = camera_handler;

public:
    using cameraman_type = base::cameraman_type;

    explicit stiff_camera_handler(cameraman_type cameraman);
    virtual ~stiff_camera_handler() override = default;

    auto mouseMoved(const OgreBites::MouseMotionEvent& e) -> bool override;
};

} // namespace input

#endif // INPUT_STIFF_CAMERA_HANDLER_HPP
