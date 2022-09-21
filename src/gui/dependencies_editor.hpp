// Contains a class responsible for the edge weights editing gui.
// Soultatos Stefanos 2022

#ifndef GUI_DEPENDENCIES_EDITOR_HPP
#define GUI_DEPENDENCIES_EDITOR_HPP

#include <boost/signals2/signal.hpp>
#include <functional>
#include <string>
#include <string_view>
#include <vector>

namespace gui
{

class dependencies_editor
{
public:
    using dependency_type = std::string_view;
    using weight_type = int;

    using weight_accessor = std::function< weight_type(dependency_type) >;

private:
    using dependency_signal
        = boost::signals2::signal< void(dependency_type, weight_type) >;

    using restore_signal = boost::signals2::signal< void() >;

public:
    using dependency_slot = dependency_signal::slot_type;
    using restore_slot = restore_signal::slot_type;
    using connection = boost::signals2::connection;

    dependencies_editor();

    auto render() const -> void;

    auto weight(dependency_type dependency) const -> weight_type;

    auto set_weights(weight_accessor f) -> void;

    auto connect_to_dependency(const dependency_slot& f) -> connection;
    auto connect_to_restore(const restore_slot& f) -> connection;

protected:
    using render_vector = std::vector< const char* >;

    auto dependencies() const -> const render_vector& { return m_dependencies; }
    auto dependencies() -> render_vector& { return m_dependencies; }

    auto emit_dependency(dependency_type val, weight_type w) const -> void;
    auto emit_restore() const -> void;

private:
    auto render_dependencies() const -> void;
    auto render_restore_button() const -> void;

    dependency_signal m_dependency_sig;
    restore_signal m_restore_sig;
    weight_accessor m_weight;

    mutable render_vector m_dependencies;
};

} // namespace gui

#endif // GUI_DEPENDENCIES_EDITOR_HPP
