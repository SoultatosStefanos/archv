#include "degrees_backend.hpp"

#include <boost/log/trivial.hpp>

namespace rendering
{

degrees_backend::degrees_backend(config_data_type degree_data)
: m_cfg { degree_data }
, m_in_degree_data { degree_data.in_data }
, m_out_degree_data { degree_data.out_data }
{
}

auto degrees_backend::connect_to_in_degree_evaluation(const slot& f)
    -> connection
{
    return m_in_degree_sig.connect(f);
}

auto degrees_backend::connect_to_out_degree_evaluation(const slot& f)
    -> connection
{
    return m_out_degree_sig.connect(f);
}

namespace
{
    inline auto must_ignore(const degree_evaluation_data& data) -> bool
    {
        return data.thresholds.light < 0
            or data.thresholds.medium <= data.thresholds.light
            or data.thresholds.heavy <= data.thresholds.medium;
    }
} // namespace

auto degrees_backend::update_in_degree_evaluation(degree_evaluation_data data)
    -> void
{
    if (must_ignore(data))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid in degree data";
        return;
    }

    m_in_degree_data = std::move(data);
    emit_in_degree_evaluation_data();
}

auto degrees_backend::update_out_degree_evaluation(degree_evaluation_data data)
    -> void
{
    if (must_ignore(data))
    {
        BOOST_LOG_TRIVIAL(warning) << "ignoring invalid out degree data";
        return;
    }

    m_out_degree_data = std::move(data);
    emit_out_degree_evaluation_data();
}

auto degrees_backend::emit_in_degree_evaluation_data() const -> void
{
    m_in_degree_sig(in_degree_data());
}

auto degrees_backend::emit_out_degree_evaluation_data() const -> void
{
    m_out_degree_sig(out_degree_data());
}

} // namespace rendering