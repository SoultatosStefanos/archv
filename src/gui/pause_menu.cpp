#include "pause_menu.hpp"

#include "view/state_machine.hpp"

#include <OGRE/OgreRoot.h>
#include <OGRE/Overlay/OgreImGuiOverlay.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <boost/log/trivial.hpp>

namespace gui
{

pause_menu::pause_menu(state_machine& sm) : m_sm { sm } { }

void pause_menu::enter()
{
    m_scene = Ogre::Root::getSingleton().getSceneManager("graph visualization");
    assert(m_scene);

    m_scene->addRenderQueueListener(Ogre::OverlaySystem::getSingletonPtr());
}

void pause_menu::exit()
{
    m_scene->removeRenderQueueListener(Ogre::OverlaySystem::getSingletonPtr());
}

void pause_menu::pause() { }

void pause_menu::resume() { }

void pause_menu::frameRendered(const Ogre::FrameEvent&)
{
    // Ogre::ImGuiOverlay::NewFrame();
}

auto pause_menu::keyPressed(const OgreBites::KeyboardEvent& e) -> bool
{
    if (e.keysym.sym == 'p')
        m_sm.fallback();

    return true;
}

} // namespace gui