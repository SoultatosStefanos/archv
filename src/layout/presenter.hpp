// Contains a class responsible for coordinating frontend objects with layout
// backend objects.
// Soultatos Stefanos 2022

#ifndef LAYOUT_PRESENTER_HPP
#define LAYOUT_PRESENTER_HPP

#include <boost/log/trivial.hpp>

namespace layout
{

template < typename View, typename Backend >
class presenter
{
public:
    using backend_type = Backend;
    using backend_layout_id_type = typename backend_type::layout_id_type;
    using backend_topology_id_type = typename backend_type::topology_id_type;
    using backend_scale_type = typename backend_type::scale_type;

    using view_type = View;
    using view_layout_type = typename view_type::layout_type;
    using view_topology_type = typename view_type::topology_type;
    using view_scale_type = typename view_type::scale_type;

    static_assert(
        std::is_convertible_v< view_layout_type, backend_layout_id_type >);
    static_assert(
        std::is_convertible_v< backend_layout_id_type, view_layout_type >);
    static_assert(
        std::is_convertible_v< view_topology_type, backend_topology_id_type >);
    static_assert(
        std::is_convertible_v< backend_topology_id_type, view_topology_type >);
    static_assert(std::is_convertible_v< view_scale_type, backend_scale_type >);
    static_assert(std::is_convertible_v< backend_scale_type, view_scale_type >);

    presenter(backend_type& backend, view_type& view)
    : m_backend { backend }, m_view { view }
    {
        initialize_view();
        install_view_responses();
    }

    auto backend() const -> const auto& { return m_backend; }
    auto view() const -> const auto& { return m_view; }

    auto fetch_layout() const -> auto { return m_backend.layout_id(); }
    auto fetch_topology() const -> auto { return m_backend.topology_id(); }
    auto fetch_scale() const -> auto { return m_backend.scale(); }

    auto select_layout(view_layout_type type) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected layout: " << type;
        m_backend.update_layout(type);
    }

    auto select_topology(view_topology_type type) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected topology: " << type;
        m_backend.update_topology(type);
    }

    auto select_scale(view_scale_type scale) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected layout scale: " << scale;
        m_backend.update_scale(scale);
    }

    auto select_restore() -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected layout restore";
        m_backend.restore();
    }

private:
    auto initialize_view() -> void
    {
        m_view.set_layout([this]() { return fetch_layout(); });
        m_view.set_topology([this]() { return fetch_topology(); });
        m_view.set_scale([this]() { return fetch_scale(); });
        BOOST_LOG_TRIVIAL(debug) << "initialized layout view";
    }

    auto install_view_responses() -> void
    {
        m_view.connect_to_layout([this](auto l) { select_layout(l); });
        m_view.connect_to_topology([this](auto s) { select_topology(s); });
        m_view.connect_to_scale([this](auto s) { select_scale(s); });
        m_view.connect_to_restore([this]() { select_restore(); });
        BOOST_LOG_TRIVIAL(debug) << "installed layout view responses";
    }

    backend_type& m_backend;
    view_type& m_view;
};

// Utility factory for type deduction.
template < typename View, typename Backend >
inline auto make_presenter(Backend& backend, View& view)
{
    return presenter< View, Backend >(backend, view);
}

} // namespace layout

#endif // LAYOUT_PRESENTER_HPP
