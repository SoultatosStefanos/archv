#include "degrees_backend.hpp"

namespace rendering
{

degrees_backend::degrees_backend(config_data_type degree_data)
: m_cfg { degree_data }
, m_in_degree_data { degree_data.first }
, m_out_degree_data { degree_data.second }
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

auto degrees_backend::update_in_degree_evaluation(degree_evaluation_data data)
    -> void
{
    m_in_degree_data = std::move(data);
    emit_in_degree_evaluation_data();
}

auto degrees_backend::update_out_degree_evaluation(degree_evaluation_data data)
    -> void
{
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