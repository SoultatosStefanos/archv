#include "mygui_manager.hpp"

#include <cassert>

namespace gui
{

auto mygui_manager::is_initialized() const -> bool
{
    return m_platform && m_gui;
}

void mygui_manager::initialize(
    Ogre::RenderWindow* win, Ogre::SceneManager* scene)
{
    assert(win);
    assert(scene);
    assert(!is_initialized());
    assert(win->hasViewportWithZOrder(0));

    m_platform = new MyGUI::OgrePlatform();
    m_gui = new MyGUI::Gui();

    m_platform->initialise(win, scene);
    m_gui->initialise();

    assert(is_initialized());
    assert(MyGUI::Gui::getInstancePtr() == m_gui);
}

namespace
{
    template < typename T >
    inline void destroy(T*& ptr)
    {
        delete ptr;
        ptr = nullptr;
    }
} // namespace

void mygui_manager::shutdown()
{
    assert(is_initialized());

    m_gui->shutdown();
    m_platform->shutdown();

    destroy(m_gui);
    destroy(m_platform);

    assert(!is_initialized());
}

mygui_manager::~mygui_manager()
{
    if (is_initialized())
        shutdown();
}

} // namespace gui