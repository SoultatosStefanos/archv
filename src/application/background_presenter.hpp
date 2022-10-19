// Contains a class responsible for connecting the background frontends.
// Soultatos Stefanos 2022

#ifndef APPLICATION_BACKGROUND_PRESENTER_HPP
#define APPLICATION_BACKGROUND_PRESENTER_HPP

#include "gui/allfwd.hpp"
#include "rendering/allfwd.hpp"

namespace application
{

class background_presenter
{
public:
    using renderer_type = rendering::background_renderer;
    using configurator_type = gui::background_configurator;

    background_presenter(renderer_type& renderer, configurator_type& cfgurator);

    auto renderer() const -> const auto& { return m_renderer; }
    auto renderer() -> auto& { return m_renderer; }

    auto configurator() const -> const auto& { return m_configurator; }
    auto configurator() -> auto& { return m_configurator; }

private:
    auto prepare_configurator() -> void;

    auto connect_configurator_with_renderer() -> void;

    renderer_type& m_renderer;
    configurator_type& m_configurator;
};

} // namespace application

#endif // APPLICATION_BACKGROUND_PRESENTER_HPP
