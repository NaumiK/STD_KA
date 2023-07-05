#include "MessageBar.h"


MessageBar::MessageBar(uint64_t messageCnt, uint64_t length, const sf::Time &dt, const sf::Vector2i &pos, int space,
                       std::string font)
        : m_message_cnt(messageCnt), m_length(length), m_delta_duration(dt), m_pos(pos), m_font(std::move(font)),
          m_space(space) {
    for (uint64_t i = 0; i < m_message_cnt; ++i) {
        m_lines.emplace_back();
        m_lines.back().first.setPosition({(float) m_pos.x, (float) (m_pos.y + m_space * i)});
        standard_text(m_lines.back().first);
    }
}

void MessageBar::update(sf::Time dt) {
    m_current_time += dt;
    for (size_t i = 0; i < m_message_cnt; ++i) {
        auto &[m_text, m_string] = m_lines[i];
        float scaledTime = (m_current_time.asSeconds() / (m_delta_duration.asSeconds() *
                                                          static_cast<float>(m_string.length())));
        auto cur_frame = static_cast<uint64_t>(scaledTime * static_cast<float>(m_length));
        m_text.setString(strRowWindow(m_string, cur_frame, m_length));
    }
}

void MessageBar::leave_message(const std::string &message) {
    for (uint64_t i = 0; i < m_message_cnt - 1; ++i) {
        m_lines[i].second = m_lines[i + 1].second;
    }
    m_lines[m_message_cnt - 1].second = (message.length() < m_length ? message : std::string(m_length, ' ') +
                                                                                 message +
                                                                                 std::string(m_length, ' '));
}

void MessageBar::standard_text(sf::Text &text) const {
    text.setFont(AssetManager::getFont(m_font));
    text.setCharacterSize(25);
}

sf::String MessageBar::strRowWindow(const std::string &str, uint64_t frame, uint64_t l) {
    if (l >= str.length()) return sf::String::fromUtf8(str.begin(), str.end());
    frame = frame % (str.length() - l + 1);
    return sf::String::fromUtf8(str.begin() + (int) frame, str.begin() + (int) (frame + l));
}

void MessageBar::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (auto &i: m_lines)
        target.draw(i.first, states);
}