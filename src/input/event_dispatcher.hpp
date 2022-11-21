// Contains a wrapper over the OgreBites::InputListener class.
// Soultatos Stefanos 2022

#ifndef INPUT_EVENT_DISPATCHER_HPP
#define INPUT_EVENT_DISPATCHER_HPP

#include <OGRE/Bites/OgreInput.h>

namespace input
{

/***********************************************************
 * Event Dispatcher                                        *
 ***********************************************************/

// Dispatches input events to a chain of input listeners.
class event_dispatcher : public OgreBites::InputListenerChain
{
    using base = OgreBites::InputListenerChain;

public:
    using chain_type = std::vector< InputListener* >;

    explicit event_dispatcher(chain_type chain = chain_type());
    ~event_dispatcher() override = default;

    auto frameRendered(const Ogre::FrameEvent& evt) -> void;
};

} // namespace input

#endif // INPUT_EVENT_DISPATCHER_HPP
