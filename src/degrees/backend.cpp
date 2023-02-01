#include "backend.hpp"

#include <boost/log/trivial.hpp>

namespace degrees
{

backend::backend(config_data_type degree_data)
: m_cfg { std::move(degree_data) }
, m_in_degree_data { m_cfg.in_data }
, m_out_degree_data { m_cfg.out_data }
{
}

auto backend::connect_to_in_degrees(const slot& f) -> connection
{
    return m_in_degree_sig.connect(f);
}

auto backend::connect_to_out_degrees(const slot& f) -> connection
{
    return m_out_degree_sig.connect(f);
}

namespace
{
    inline auto must_ignore(const evaluation_data& data) -> bool
    {
        return data.thresholds.light < 0
            or data.thresholds.medium <= data.thresholds.light
            or data.thresholds.heavy <= data.thresholds.medium;
    }
} // namespace

auto backend::update_in_degrees(evaluation_data data) -> void
{
    if (must_ignore(data))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid in degree data";
        return;
    }

    m_in_degree_data = std::move(data);
    emit_in_degrees_data();
}

auto backend::update_out_degrees(evaluation_data data) -> void
{
    if (must_ignore(data))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid out degree data";
        return;
    }

    m_out_degree_data = std::move(data);
    emit_out_degrees_data();
}

auto backend::emit_in_degrees_data() const -> void
{
    m_in_degree_sig(in_degree_data());
}

auto backend::emit_out_degrees_data() const -> void
{
    m_out_degree_sig(out_degree_data());
}

} // namespace degrees