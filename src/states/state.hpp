// Contains an interface for an application's scene.
// Soultatos Stefanos 2022

#ifndef STATES_STATE_HPP
#define STATES_STATE_HPP

#include <OGRE/Bites/OgreInput.h>

namespace states
{

class state : public OgreBites::InputListener
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

} // namespace states

#endif // STATES_STATE_HPP
