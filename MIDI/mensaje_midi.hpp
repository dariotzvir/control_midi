//
// Created by dario on 10/9/22.
//

#ifndef CONTROL_MIDI_MENSAJE_MIDI_HPP
#define CONTROL_MIDI_MENSAJE_MIDI_HPP

#include <cinttypes>
#include <array>
#include <string>
#include <string_view>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

namespace Formatear_Midi
{
    using status_types = std::map<const uint8_t, std::string_view>;
    static constexpr std::string_view NOTE_OFF       = "Note Off        ";
    static constexpr std::string_view NOTE_ON        = "Note On         ";
    static constexpr std::string_view KEY_PRESSURE   = "Key Pressure    ";
    static constexpr std::string_view CTRL_CHANGE    = "Control Change  ";
    static constexpr std::string_view PGRM_CHANGE    = "Program Change  ";
    static constexpr std::string_view CHAN_PRESSURE  = "Channel pressure";
    static constexpr std::string_view PITCH_BEND     = "Pitch Bend      ";
    static constexpr std::string_view SYS_ESCLUSIVE  = "System exclusive";
    static status_types status_bytes =
    {
        {0x80, NOTE_OFF} ,
        {0x90, NOTE_ON},
        {0xA0, KEY_PRESSURE} ,
        {0xB0, CTRL_CHANGE},
        {0xC0, PGRM_CHANGE},
        {0xD0, CHAN_PRESSURE},
        {0xE0, PITCH_BEND},
        {0xF0, SYS_ESCLUSIVE}
    };

    class Mensaje
    {
        const uint8_t _status;
        const std::vector<uint8_t> _datos;

        [[nodiscard]] std::string      hex_a_nota() const;
        [[nodiscard]] uint16_t         formatear_pitch() const;
    public:
        explicit Mensaje(uint8_t status, std::vector<uint8_t> &datos);
        [[nodiscard]] std::string_view status() const;
        [[nodiscard]] uint8_t          canal() const;
        [[nodiscard]] std::string      string() const;
    };

}


#endif //CONTROL_MIDI_MENSAJE_MIDI_HPP
