// Contains an interface for an application's state.
// Soultatos Stefanos 2022

#ifndef APPLICATION_STATE_HPP
#define APPLICATION_STATE_HPP

#include <OGRE/Bites/OgreInput.h>
#include <OGRE/OgreFrameListener.h>

namespace application
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

} // namespace application

#endif // APPLICATION_STATE_HPP
