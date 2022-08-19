// Contains a progress bar widget adaptor.
// Soultatos Stefanos 2022

#ifndef GUI_PROGRESS_BAR_HPP
#define GUI_PROGRESS_BAR_HPP

#include "overlay.hpp"

#include <string>

namespace gui
{

// Draws at a new window.
class progress_bar : public overlay
{
public:
    using fraction_type = float;
    using caption_type = std::string;

    explicit progress_bar(caption_type caption = "Please wait...");
    virtual ~progress_bar() = default;

    auto progress() const -> const fraction_type& { return m_progress; }
    auto progress() -> fraction_type& { return m_progress; }

    auto caption() const -> const caption_type& { return m_caption; }
    auto caption() -> caption_type& { return m_caption; }

    virtual auto draw() const -> void override;

private:
    fraction_type m_progress;
    caption_type m_caption;
};

} // namespace gui

#endif // GUI_PROGRESS_BAR_HPP
