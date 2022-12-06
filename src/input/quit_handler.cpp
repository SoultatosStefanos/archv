#include "quit_handler.hpp"

#include <OGRE/OgreRoot.h>

namespace input
{

quit_handler::quit_handler(root_type& root) : m_root { root }
{
}

auto quit_handler::keyReleased(const OgreBites::KeyboardEvent& e) -> bool
{
    if (e.keysym.sym == OgreBites::SDLK_ESCAPE)
        root().queueEndRendering();

    return base::keyReleased(e);
}

} // namespace input