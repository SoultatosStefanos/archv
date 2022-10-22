// Contains a class responsible for pausing/resuming the application.
// Soultatos Stefanos 2022

#ifndef INPUT_PAUSE_RESUME_HANDLER_HPP
#define INPUT_PAUSE_RESUME_HANDLER_HPP

#include <OGRE/Bites/OgreApplicationContext.h>
#include <OGRE/Bites/OgreCameraMan.h>
#include <OGRE/Bites/OgreImGuiInputListener.h>
#include <OGRE/Bites/OgreInput.h>

namespace input
{

class pause_resume_handler : public OgreBites::InputListener
{
    using base = OgreBites::InputListener;

public:
    using application_type = OgreBites::ApplicationContext;
    using cameraman_type = OgreBites::CameraMan;
    using gui_handler_type = OgreBites::ImGuiInputListener;

    pause_resume_handler(
        application_type& app,
        cameraman_type& cameraman,
        gui_handler_type& gui_handler);

    virtual ~pause_resume_handler() override = default;

    auto paused() const -> bool { return m_paused; }

    auto app() const -> const auto& { return m_app; }
    auto app() -> auto& { return m_app; }

    auto cameraman() const -> const auto& { return m_cameraman; }
    auto cameraman() -> auto& { return m_cameraman; }

    auto gui_handler() const -> const auto& { return m_gui_handler; }
    auto gui_handler() -> auto& { return m_gui_handler; }

    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;

private:
    auto toggle_pause_resume() -> void;

    auto pause() -> void;
    auto resume() -> void;

    bool m_paused = false;
    application_type& m_app;
    cameraman_type& m_cameraman;
    gui_handler_type& m_gui_handler;
};

} // namespace input

#endif // INPUT_PAUSE_RESUME_HANDLER_HPP
