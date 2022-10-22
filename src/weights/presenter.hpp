// Contains a class responsible for connecting the weights management frontend
// and backend objects.
// Soultatos Stefanos 2022

#ifndef WEIGHTS_PRESENTER_HPP
#define WEIGHTS_PRESENTER_HPP

#include <boost/log/trivial.hpp>

namespace weights
{

template < typename View, typename Backend >
class presenter
{
public:
    using backend_type = Backend;
    using backend_dependency_type = typename backend_type::dependency_type;
    using backend_weight_type = typename backend_type::weight_type;

    using view_type = View;
    using view_dependency_type = typename view_type::dependency_type;
    using view_weight_type = typename view_type::weight_type;

    static_assert(
        std::is_convertible_v< backend_dependency_type, view_dependency_type >);
    static_assert(
        std::is_convertible_v< view_dependency_type, backend_dependency_type >);
    static_assert(
        std::is_convertible_v< backend_weight_type, view_weight_type >);
    static_assert(
        std::is_convertible_v< view_weight_type, backend_weight_type >);

    presenter(backend_type& back, view_type& view)
    : m_backend { back }, m_view { view }
    {
        initialize_view();
        install_view_responses();
    }

    auto backend() const -> const auto& { return m_backend; }
    auto view() const -> const auto& { return m_view; }

    auto fetch_weight(view_dependency_type type) const -> backend_weight_type
    {
        return m_backend.weight(type);
    }

    auto select_weight(view_dependency_type type, view_weight_type w) -> void
    {
        BOOST_LOG_TRIVIAL(info)
            << "selected weight: " << w << " for dependency: " << w;
        m_backend.update_weight(type, w);
    }

    auto select_restore() -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected weights restore";
        m_backend.restore();
    }

private:
    auto initialize_view() -> void
    {
        m_view.set_weights([this](auto type) { return fetch_weight(type); });
        BOOST_LOG_TRIVIAL(debug) << "initialized weights view";
    }

    auto install_view_responses() -> void
    {
        m_view.connect_to_dependency([this](auto type, auto w)
                                     { select_weight(type, w); });
        m_view.connect_to_restore([this]() { select_restore(); });
        BOOST_LOG_TRIVIAL(debug) << "installed weights view responses";
    }

    backend_type& m_backend;
    view_type& m_view;
};

// Utility factory for type deduction.
template < typename View, typename Backend >
inline auto make_presenter(Backend& back, View& view)
{
    return presenter< View, Backend >(back, view);
}

} // namespace weights

#endif // WEIGHTS_PRESENTER_HPP
