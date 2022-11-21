#include "event_dispatcher.hpp"

#include <cassert>

namespace input
{

event_dispatcher::event_dispatcher(chain_type chain) : base(std::move(chain))
{
}

auto event_dispatcher::frameRendered(const Ogre::FrameEvent& evt) -> void
{
    for (auto* listener : mListenerChain)
    {
        assert(listener);
        listener->frameRendered(evt);
    }
}

} // namespace input