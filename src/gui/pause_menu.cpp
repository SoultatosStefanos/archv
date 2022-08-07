#include "pause_menu.hpp"

#include <cassert>

namespace gui
{

namespace
{

    auto native_gui() -> auto&
    {
        assert(MyGUI::Gui::getInstancePtr());
        return MyGUI::Gui::getInstance();
    }

} // namespace

// TODO Cleanup
pause_menu::pause_menu(
    layout_options_set layout_options,
    topology_options_set topology_options,
    scale_options_set scale_options)
: m_layout_options { std::move(layout_options) }
, m_topology_options { topology_options }
, m_scale_options { scale_options }
{
    // Menubar

    auto* menubar = native_gui().createWidget< MyGUI::MenuBar >(
        "MenuBar",
        20,
        40,
        1880,
        60,
        MyGUI::Align::HStretch | MyGUI::Align::Top,
        "Main");

    // Layout

    auto* layout_btn
        = menubar->addItem("Layout / Topology", MyGUI::MenuItemType::Popup);

    auto* p = layout_btn->createWidget< MyGUI::PopupMenu >(
        MyGUI::WidgetStyle::Popup,
        "PopupMenu",
        MyGUI::IntCoord(),
        MyGUI::Align::Default,
        "Popup");

    p->setPopupAccept(true);

    auto* la = p->addItem("Layout", MyGUI::MenuItemType::Popup);

    auto* p1 = la->createWidget< MyGUI::PopupMenu >(
        MyGUI::WidgetStyle::Popup,
        "PopupMenu",
        MyGUI::IntCoord(),
        MyGUI::Align::Default,
        "Popup");

    p1->setPopupAccept(true);

    for (const auto& layout_option : m_layout_options)
    {
        auto* i = p1->addItem(layout_option, MyGUI::MenuItemType::Normal);

        i->eventMouseButtonClick = MyGUI::newDelegate(this, &emit_layout);
    }

    auto* tt = p->addItem("Topology", MyGUI::MenuItemType::Popup);

    auto* p2 = tt->createWidget< MyGUI::PopupMenu >(
        MyGUI::WidgetStyle::Popup,
        "PopupMenu",
        MyGUI::IntCoord(),
        MyGUI::Align::Default,
        "Popup");

    p2->setPopupAccept(true);

    for (const auto& topology_option : m_topology_options)
    {
        auto* i = p2->addItem(topology_option, MyGUI::MenuItemType::Normal);

        i->eventMouseButtonClick = MyGUI::newDelegate(this, &emit_topology);
    }

    auto* ts = p->addItem("Scale", MyGUI::MenuItemType::Popup);

    auto* p3 = ts->createWidget< MyGUI::PopupMenu >(
        MyGUI::WidgetStyle::Popup,
        "PopupMenu",
        MyGUI::IntCoord(),
        MyGUI::Align::Default,
        "Popup");

    p3->setPopupAccept(true);

    for (auto scale_option : m_scale_options)
    {
        const auto scale_str = std::to_string(scale_option);
        auto* i = p3->addItem(scale_str, MyGUI::MenuItemType::Normal);

        i->eventMouseButtonClick = MyGUI::newDelegate(this, &emit_scale);
    }

    // Rest

    auto* clustering_btn
        = menubar->addItem("Clustering", MyGUI::MenuItemType::Popup);
    auto* code_btn
        = menubar->addItem("Code Inspection", MyGUI::MenuItemType::Popup);
    auto* settings_btn
        = menubar->addItem("Settings", MyGUI::MenuItemType::Popup);

    auto* s1 = menubar->addItem("Seperator1", MyGUI::MenuItemType::Separator);
    auto* s2 = menubar->addItem("Seperator2", MyGUI::MenuItemType::Separator);
    auto* s3 = menubar->addItem("Seperator3", MyGUI::MenuItemType::Separator);
    auto* s4 = menubar->addItem("Seperator4", MyGUI::MenuItemType::Separator);

    // Coords

    layout_btn->setCoord(0, 0, 270, 60);
    clustering_btn->setCoord(270, 0, 270, 60);
    code_btn->setCoord(540, 0, 270, 60);
    settings_btn->setCoord(1630, 0, 270, 60);

    s1->setCoord(270, 10, 40, 40);
    s2->setCoord(540, 10, 40, 40);
    s3->setCoord(810, 10, 40, 40);
    s4->setCoord(1630, 10, 40, 40);
}

// TODO
pause_menu::~pause_menu()
{
    // Cleanup with care
}

void pause_menu::emit_layout(MyGUI::Widget* from) const
{
    assert(dynamic_cast< MyGUI::MenuItem* >(from));

    const auto* item = static_cast< MyGUI::MenuItem* >(from);

    m_layout_sig(item->getCaption());
}

void pause_menu::emit_topology(MyGUI::Widget* from) const
{
    assert(dynamic_cast< MyGUI::MenuItem* >(from));

    const auto* item = static_cast< MyGUI::MenuItem* >(from);

    m_topology_sig(item->getCaption());
}

void pause_menu::emit_scale(MyGUI::Widget* from) const
{
    assert(dynamic_cast< MyGUI::MenuItem* >(from));

    const auto* item = static_cast< MyGUI::MenuItem* >(from);
    const auto scale = std::stod(item->getCaption().asUTF8());

    m_scale_sig(scale);
}

} // namespace gui