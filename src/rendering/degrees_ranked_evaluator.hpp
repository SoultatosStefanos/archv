// Contains the static default vertex degree evaluation policy class.
// Soultatos Stefanos 2022

#ifndef RENDERING_DEGREES_RANKED_EVALUATOR_HPP
#define RENDERING_DEGREES_RANKED_EVALUATOR_HPP

#include "degrees_ranked_backend.hpp"    // for degrees_ranked_backend
#include "degrees_ranked_evaluation.hpp" // for degree_t, particle_system_t

namespace rendering
{

// Graph vertex degree evaluation policy based on ranked thresholds.
// Proxy to the degrees evaluation backend.
class degrees_ranked_evaluator
{
public:
    using degree_type = degree_t;
    using particles_type = particle_system_t;
    using backend_type = degrees_ranked_backend;

    explicit degrees_ranked_evaluator(backend_type backend = backend_type())
    : m_backend { std::move(backend) }
    {
    }

    auto backend() const -> const auto& { return m_backend; }
    auto backend() -> auto& { return m_backend; }

    inline auto in_degree_particles(degree_type degree) const -> particles_type
    {
        return evaluate_in_degree(backend(), degree);
    }

    inline auto out_degree_particles(degree_type degree) const -> particles_type
    {
        return evaluate_out_degree(backend(), degree);
    }

private:
    backend_type m_backend;
};

} // namespace rendering

#endif // RENDERING_DEGREES_RANKED_EVALUATOR_HPP
