// Contains the static default vertex degree evaluation policy class.
// Proxy object to use as an evaluation policy at a renderer.
// Soultatos Stefanos 2022

#ifndef DEGREES_EVALUATOR_HPP
#define DEGREES_EVALUATOR_HPP

#include "evaluation.hpp" // for degree_t, particle_system_t, backend

#include <string> // for string

namespace degrees
{

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

    explicit evaluator(const backend_type& back) : m_backend { back } { }

    inline auto in_degree_particles(degree_type degree) const -> particles_type
    {
        return evaluate_in_degree(m_backend, degree);
    }

    inline auto out_degree_particles(degree_type degree) const -> particles_type
    {
        return evaluate_out_degree(m_backend, degree);
    }

private:
    const backend_type& m_backend;
};

} // namespace degrees

#endif // DEGREES_EVALUATOR_HPP
