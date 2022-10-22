// Contains an input listener responsible for quitting the application.
// Soultatos Stefanos 2022

#ifndef INPUT_QUIT_HANDLER_HPP
#define INPUT_QUIT_HANDLER_HPP

#include <OGRE/Bites/OgreInput.h>
#include <OGRE/OgreRoot.h>

namespace input
{

class quit_handler : public OgreBites::InputListener
{
    using base = OgreBites::InputListener;

public:
    using root_type = Ogre::Root;

    explicit quit_handler(root_type& root);

    auto root() const -> const auto& { return m_root; }
    auto root() -> auto& { return m_root; }

    auto keyReleased(const OgreBites::KeyboardEvent& e) -> bool override;

private:
    root_type& m_root;
};

} // namespace input

#endif // INPUT_QUIT_HANDLER_HPP
