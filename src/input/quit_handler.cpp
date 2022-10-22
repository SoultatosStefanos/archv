#include "quit_handler.hpp"

namespace input
{

quit_handler::quit_handler(root_type& root) : m_root { root }
{
}

auto quit_handler::keyReleased(const OgreBites::KeyboardEvent& e) -> bool
{
    if (e.keysym.sym == OgreBites::SDLK_ESCAPE)
        root().queueEndRendering();
    return true;
}

} // namespace input