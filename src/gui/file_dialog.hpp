// Contains the file dialog of the application.
// Soultatos Stefanos 2022

#ifndef GUI_FILE_DIALOG_HPP
#define GUI_FILE_DIALOG_HPP

#include "detail/imfilebrowser.h"
#include "overlay.hpp"

#include <boost/signals2/signal.hpp>
#include <string>

namespace gui
{

class file_dialog : public overlay
{
    using signal = boost::signals2::signal< void(const std::string&) >;

public:
    using slot = signal::slot_type;
    using connection = boost::signals2::connection;

    file_dialog();
    virtual ~file_dialog() = default;

    // NOTE:: MUST be called at each frame.
    virtual auto draw() const -> void override;

    auto open() -> void;

    auto connect(const slot& f) -> connection { return m_signal.connect(f); }

private:
    mutable ImGui::FileBrowser m_impl;
    signal m_signal;
};

} // namespace gui

#endif // GUI_FILE_DIALOG_HPP
