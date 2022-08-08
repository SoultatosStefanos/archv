// Contains an interface for an application's view state.
// Soultatos Stefanos 2022

#ifndef VIEW_STATE_HPP
#define VIEW_STATE_HPP

#include <OGRE/Bites/OgreInput.h>
#include <OGRE/OgreFrameListener.h>

namespace view
{

class state : public OgreBites::InputListener, public Ogre::FrameListener
{
public:
    state() = default;
    state(const state&) = default;
    state(state&&) = default;
    virtual ~state() = default;

    auto operator=(const state&) -> state& = default;
    auto operator=(state&&) -> state& = default;

    virtual void enter() = 0;
    virtual void exit() = 0;

    virtual void pause() = 0;
    virtual void resume() = 0;
};

} // namespace view

#endif // VIEW_STATE_HPP
