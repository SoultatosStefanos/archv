// Contains the static default vertex degree evaluation policy class.
// Proxy object to use as an evaluation policy at a renderer.
// Soultatos Stefanos 2022

#ifndef DEGREES_EVALUATOR_HPP
#define DEGREES_EVALUATOR_HPP

#include <string> // for string

namespace degrees
{

/***********************************************************
 * Fwd declarations                                        *
 ***********************************************************/

using degree_t = int;
using particle_system_t = std::string;

class backend;

extern auto evaluate_in_degree(const backend&, degree_t) -> particle_system_t;
extern auto evaluate_out_degree(const backend&, degree_t) -> particle_system_t;

/***********************************************************
 * Evaluator                                               *
 ***********************************************************/

// Graph vertex degree evaluation policy based on ranked thresholds.
// Proxy to the degrees evaluation backend.
class evaluator
{
public:
    using degree_type = degree_t;
    using particles_type = particle_system_t;
    using backend_type = backend;

    explicit evaluator(const backend_type& backend) : m_backend { backend } { }

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
    const backend_type& m_backend;
};

} // namespace degrees

#endif // DEGREES_EVALUATOR_HPP
