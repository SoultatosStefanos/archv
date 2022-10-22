// Contains a class responsible for coordinating frontend objects with scaling
// backend objects.
// Soultatos Stefanos 2022

#ifndef SCALING_PRESENTER_HPP
#define SCALING_PRESENTER_HPP

#include <boost/log/trivial.hpp>

namespace scaling
{

template < typename View, typename Backend >
class presenter
{
public:
    using backend_type = Backend;
    using backend_tag_type = typename backend_type::tag_type;
    using backend_dims_type = typename backend_type::dims_type;
    using backend_baseline_type = typename backend_type::baseline_type;
    using backend_enabled_type = typename backend_type::enabled_type;
    using backend_ratio_type = typename backend_type::ratio_type;

    using view_type = View;
    using view_tag_type = typename view_type::tag_type;
    using view_dims_type = typename view_type::dims_type;
    using view_baseline_type = typename view_type::baseline_type;
    using view_enabled_type = typename view_type::enabled_type;
    using view_ratio_type = typename view_type::ratio_type;

    static_assert(std::is_convertible_v< backend_tag_type, view_tag_type >);
    static_assert(std::is_convertible_v< backend_dims_type, view_dims_type >);
    static_assert(
        std::is_convertible_v< backend_baseline_type, view_baseline_type >);
    static_assert(
        std::is_convertible_v< backend_enabled_type, view_enabled_type >);
    static_assert(std::is_convertible_v< backend_ratio_type, view_ratio_type >);

    static_assert(std::is_convertible_v< view_tag_type, backend_tag_type >);
    static_assert(std::is_convertible_v< view_dims_type, backend_dims_type >);
    static_assert(
        std::is_convertible_v< view_baseline_type, backend_baseline_type >);
    static_assert(
        std::is_convertible_v< view_enabled_type, backend_enabled_type >);
    static_assert(std::is_convertible_v< view_ratio_type, backend_ratio_type >);

    presenter(backend_type& back, view_type& view)
    : m_backend { back }, m_view { view }
    {
        initialize_view();
        install_view_responses();
    }

    auto fetch_factor_baseline(view_tag_type t) const -> backend_baseline_type
    {
        return m_backend.factor_baseline(t);
    }

    auto fetch_factor_dims(view_tag_type t) const -> backend_dims_type
    {
        return m_backend.factor_dims(t);
    }

    auto fetch_factor_enabled(view_tag_type t) const -> backend_baseline_type
    {
        return m_backend.factor_enabled(t);
    }

    auto fetch_factor_min_ratio(view_tag_type t) const -> backend_ratio_type
    {
        return m_backend.factor_min_ratio(t);
    }

    auto fetch_factor_max_ratio(view_tag_type t) const -> backend_ratio_type
    {
        return m_backend.factor_max_ratio(t);
    }

    auto select_factor_baseline(view_tag_type t, view_baseline_type b) -> void
    {
        BOOST_LOG_TRIVIAL(info)
            << "selected scaling baseline: " << b << " for: " << t;
        m_backend.update_factor_baseline(t, b);
    }

    auto select_factor_dims(view_tag_type t, view_dims_type dims) -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected scaling dims for: " << t;
        m_backend.update_factor_dims(t, dims);
    }

    auto select_factor_enabled(view_tag_type t, view_enabled_type v) -> void
    {
        BOOST_LOG_TRIVIAL(info)
            << "selected scaling enabled: " << v << " for: " << t;
        m_backend.update_factor_enabled(t, v);
    }

    auto select_factor_min_ratio(view_tag_type t, view_ratio_type r) -> void
    {
        BOOST_LOG_TRIVIAL(info)
            << "selected scaling min ratio: " << r << " for: " << t;
        m_backend.update_factor_min_ratio(t, r);
    }

    auto select_factor_max_ratio(view_tag_type t, view_ratio_type r) -> void
    {
        BOOST_LOG_TRIVIAL(info)
            << "selected scaling max ratio: " << r << " for: " << t;
        m_backend.update_factor_max_ratio(t, r);
    }

    auto select_restore() -> void
    {
        BOOST_LOG_TRIVIAL(info) << "selected scaling restore";
        m_backend.restore();
    }

private:
    auto initialize_view() -> void
    {
        m_view.set_baseline([this](auto tag)
                            { return fetch_factor_baseline(tag); });
        m_view.set_dims([this](auto tag) { return fetch_factor_dims(tag); });
        m_view.set_enabled([this](auto tag)
                           { return fetch_factor_enabled(tag); });
        m_view.set_min_ratio([this](auto tag)
                             { return fetch_factor_min_ratio(tag); });
        m_view.set_max_ratio([this](auto tag)
                             { return fetch_factor_max_ratio(tag); });

        BOOST_LOG_TRIVIAL(debug) << "initialized scaling view";
    }

    auto install_view_responses() -> void
    {
        m_view.connect_to_baseline([this](auto tag, auto base)
                                   { select_factor_baseline(tag, base); });

        m_view.connect_to_dims([this](auto tag, const auto& dims)
                               { select_factor_dims(tag, dims); });

        m_view.connect_to_enabled([this](auto tag, auto enabled)
                                  { select_factor_enabled(tag, enabled); });

        m_view.connect_to_min_ratio([this](auto tag, auto ratio)
                                    { select_factor_min_ratio(tag, ratio); });

        m_view.connect_to_max_ratio([this](auto tag, auto ratio)
                                    { select_factor_max_ratio(tag, ratio); });

        m_view.connect_to_restore([this]() { select_restore(); });

        BOOST_LOG_TRIVIAL(debug) << "installed scaling view responses";
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

} // namespace scaling

#endif // SCALING_PRESENTER_HPP
