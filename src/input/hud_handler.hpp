// Contains a class responsible for dispatching input to hud elements.
// Soultatos Stefanos 2022

#ifndef INPUT_HUD_HANDLER_HPP
#define INPUT_HUD_HANDLER_HPP

#include <OGRE/Bites/OgreInput.h>
#include <OGRE/Bites/OgreTrays.h>

namespace input
{

class hud_handler : public OgreBites::InputListener
{
    using base = OgreBites::InputListener;

public:
    using tray_type = OgreBites::TrayManager;

    explicit hud_handler(tray_type& tray);

    auto tray() const -> const auto& { return m_tray; }
    auto tray() -> auto& { return m_tray; }

    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;

private:
    auto toggle_frame_stats() -> void;

    tray_type& m_tray;
};

} // namespace input

#endif // INPUT_HUD_HANDLER_HPP
