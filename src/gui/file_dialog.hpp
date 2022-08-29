// Contains the file dialog of the application.
// Soultatos Stefanos 2022

#ifndef GUI_FILE_DIALOG_HPP
#define GUI_FILE_DIALOG_HPP

#include "overlay.hpp"

#include <boost/signals2/signal.hpp>
#include <memory>
#include <string>

namespace ImGui
{
class FileBrowser;
}

namespace gui
{

class file_dialog : public overlay
{
    using signal = boost::signals2::signal< void(const std::string&) >;

public:
    using slot = signal::slot_type;
    using connection = boost::signals2::connection;

    file_dialog();
    ~file_dialog();

    file_dialog(const file_dialog&) = delete;
    file_dialog(file_dialog&&) = delete;

    auto operator=(const file_dialog&) -> file_dialog& = delete;
    auto operator=(file_dialog&&) -> file_dialog& = delete;

    // NOTE:: MUST be called at each frame.
    virtual auto draw() const -> void override;

    auto open() -> void;

    auto connect(const slot& f) -> connection { return m_signal.connect(f); }

private:
    // use of pimpl cause redefinition errors
    std::unique_ptr< ImGui::FileBrowser > m_pimpl;
    signal m_signal;
};

} // namespace gui

#endif // GUI_FILE_DIALOG_HPP
