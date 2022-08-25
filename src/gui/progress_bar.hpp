// Contains a progress bar widget adaptor.
// Soultatos Stefanos 2022

#ifndef GUI_PROGRESS_BAR_HPP
#define GUI_PROGRESS_BAR_HPP

#include "overlay.hpp"

#include <atomic>
#include <boost/signals2/signal.hpp>
#include <string>

namespace gui
{

class progress_bar : public overlay
{
    using cancel_signal = boost::signals2::signal< void() >;

public:
    using fraction_type = float;
    using caption_type = std::string;

    using cancel_slot = cancel_signal::slot_type;
    using connection = boost::signals2::connection;

    virtual ~progress_bar() = default;

    auto progress() const -> const auto& { return m_progress; }
    auto progress() -> auto& { return m_progress; }

    // NOTE:: Not thread safe.
    auto caption() const -> const caption_type& { return m_caption; }
    auto caption() -> caption_type& { return m_caption; }

    virtual auto draw() const -> void override;

    auto connect_to_cancel(const cancel_slot& f) -> connection
    {
        return m_cancel.connect(f);
    }

private:
    std::atomic< fraction_type > m_progress;
    caption_type m_caption;

    cancel_signal m_cancel;
};

} // namespace gui

#endif // GUI_PROGRESS_BAR_HPP
