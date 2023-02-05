// Contains the file browser menu of the gui.
// Soultatos Stefanos 2022

#ifndef GUI_FILE_BROWSER_HPP
#define GUI_FILE_BROWSER_HPP

#include <boost/signals2/signal.hpp>
#include <memory>
#include <string_view>

namespace ImGui
{
class FileBrowser;
} // namespace ImGui

namespace gui
{

class file_browser
{
public:
    using file_type = std::string_view;

private:
    using file_signal = boost::signals2::signal< void(file_type) >;

public:
    using file_slot = file_signal::slot_type;
    using connection = boost::signals2::connection;

    file_browser();
    ~file_browser();

    // NOTE: Call at each frame.
    auto render() const -> void;

    auto open() -> void;

    auto connect(const file_slot& slot) -> connection;

protected:
    auto emit_file(file_type file) const -> void;

private:
    using impl_type = ImGui::FileBrowser;

    file_signal m_file_sig;

    // NOTE: pimpl because of 3rd party implementation conflicting includes
    std::unique_ptr< impl_type > m_impl;
};

} // namespace gui

#endif // GUI_FILE_BROWSER_HPP
