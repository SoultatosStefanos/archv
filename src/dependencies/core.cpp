#include "core.hpp"

#include <boost/log/trivial.hpp>

namespace dependencies::detail
{

/***********************************************************
 * Update Weight Use Case                                  *
 ***********************************************************/

class update_weight_command : public utility::command
{
public:
    using signal = update_weight_service::signal;
    using dependency_type = update_weight_service::dependency_type;
    using weight = update_weight_service::weight;

    update_weight_command(
        signal& sig, weight_repo& repo, dependency_type type, weight score);

    virtual ~update_weight_command() override = default;

    virtual void execute() override;
    virtual void undo() override;
    virtual void redo() override { execute(); }

    virtual auto clone() const -> std::unique_ptr< command > override
    {
        return std::make_unique< update_weight_command >(*this);
    }

private:
    void set_weight(const dependency_type& type, weight score);

    signal& m_sig;
    weight_repo& m_repo;
    dependency_type m_dependency;
    weight m_score, m_prev_score;
};

update_weight_command::update_weight_command(
    signal& sig, weight_repo& repo, dependency_type type, weight score)
: m_sig { sig }
, m_repo { repo }
, m_dependency { std::move(type) }
, m_score { score }
, m_prev_score { m_repo.get_weight(m_dependency) }
{
}

void update_weight_command::execute()
{
    if (m_repo.get_weight(m_dependency) != m_score)
        set_weight(m_dependency, m_score);
}

void update_weight_command::undo()
{
    if (m_repo.get_weight(m_dependency) != m_prev_score)
        set_weight(m_dependency, m_prev_score);
}

void update_weight_command::set_weight(
    const dependency_type& type, weight score)
{
    m_repo.set_weight(type, score);

    BOOST_LOG_TRIVIAL(info)
        << "dependency type: " << type << " assigned with weight: " << score;

    m_sig(type, score);
}

update_weight_service::update_weight_service(
    command_history& cmds, weight_repo& repo)
: m_cmds { cmds }, m_repo { repo }
{
}

void update_weight_service::operator()(
    const dependency_type& type, weight score)
{
    m_cmds.execute(std::make_unique< update_weight_command >(
        m_signal, m_repo, type, score));
}

/***********************************************************
 * Revert To Defaults Use Case                             *
 ***********************************************************/

class revert_to_defaults_command : public utility::command
{
public:
    using signal = revert_to_defaults_service::signal;

    revert_to_defaults_command(
        signal& sig, weight_repo& repo, weight_repo initial_repo);

    virtual ~revert_to_defaults_command() override = default;

    virtual void execute() override;
    virtual void undo() override;
    virtual void redo() override { execute(); }

    virtual auto clone() const -> std::unique_ptr< command > override
    {
        return std::make_unique< revert_to_defaults_command >(*this);
    }

private:
    void revert(const weight_repo& copy);

    signal& m_sig;
    weight_repo& m_repo;
    weight_repo m_initial_repo, m_prev_repo;
};

revert_to_defaults_command::revert_to_defaults_command(
    signal& sig, weight_repo& repo, weight_repo initial_repo)
: m_sig { sig }
, m_repo { repo }
, m_initial_repo { std::move(initial_repo) }
, m_prev_repo { repo }
{
}

void revert_to_defaults_command::execute() { revert(m_initial_repo); }

void revert_to_defaults_command::undo() { revert(m_prev_repo); }

void revert_to_defaults_command::revert(const weight_repo& copy)
{
    m_repo = copy;

    BOOST_LOG_TRIVIAL(info) << "restored original dependencies weights";

    for (const auto& [d, w] : m_repo)
        m_sig(d, w);
}

revert_to_defaults_service::revert_to_defaults_service(
    command_history& cmds, weight_repo& repo)
: m_cmds { cmds }, m_repo { repo }, m_initial_repo(repo)
{
}

void revert_to_defaults_service::operator()()
{
    m_cmds.execute(std::make_unique< revert_to_defaults_command >(
        m_signal, m_repo, m_initial_repo));
}

} // namespace dependencies::detail

namespace dependencies
{

core::core(command_history& cmds, hash_table table)
: m_repo { std::move(table) }
, m_update_weight { cmds, m_repo }
, m_revert_to_defaults { cmds, m_repo }
{
    m_update_weight.connect([this](const auto& d, const auto& w)
                            { m_signal(d, w); });

    m_revert_to_defaults.connect([this](const auto& d, const auto& w)
                                 { m_signal(d, w); });
}

void core::update_weight(const dependency_type& type, weight score)
{
    m_update_weight(type, score);
}

void core::revert_to_defaults() { m_revert_to_defaults(); }

} // namespace dependencies