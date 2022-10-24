//
// Created by dario on 10/9/22.
//

#include "mensaje_midi.hpp"

namespace Formatear_Midi
{
    Mensaje::Mensaje(uint8_t status, std::vector<uint8_t> &datos, double dif_tiempo):
    _status(status), _datos(std::move(datos)), _dif_tiempo(dif_tiempo){};
    [[nodiscard]] std::string_view Mensaje::status() const
    {
        const uint8_t status = (_status & 0b11110000);
        return codigo_status.at(status); //codigo_status[status];
    }
    [[nodiscard]] uint8_t Mensaje::canal() const
    {
        if(status() == SYS_ESCLUSIVE) return 0;
        return (_status & 0b00001111);
    }
    [[nodiscard]] std::string Mensaje::string() const
    {
        std::string r;
        std::string_view status_aux = status();
        r.append(status_aux);
        r += " | Canal: " + std::to_string(canal() + 1) + " | ";

        if(status_aux == SYS_ESCLUSIVE)
        {
            char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

            r += hex_chars[ ( _status & 0xF0 ) >> 4 ];
            r += hex_chars[ ( _status & 0x0F ) >> 0 ];
            r += " ";

            for(uint8_t dato: _datos)
            {
                r += hex_chars[ ( dato & 0xF0 ) >> 4 ];
                r += hex_chars[ ( dato & 0x0F ) >> 0 ];
                r += " ";
            }
        }

        else if(status_aux == CTRL_CHANGE)
        {
            auto control = _datos[0];
            uint8_t valor   = _datos[1];

            r += std::to_string(control);
            r.append(" ");
            r += std::to_string(valor);
        }
        else if(status_aux == PGRM_CHANGE)
        {
            auto programa = _datos[0];
            r += std::to_string(programa);
        }
        else if(status_aux == PITCH_BEND)
        {
            auto pitch = formatear_pitch();
            r += std::to_string(pitch);
        }
        else
        {
            auto nota = hex_a_nota();
            uint8_t velocidad   = _datos[1];

            r += nota;
            r.append(" ");
            r += std::to_string(velocidad);
        }
        return r;
    }
    //From: JUCE https://github.com/juce-framework/JUCE
    [[nodiscard]] std::string Mensaje::hex_a_nota() const
    {
        static const char* const sharpNoteNames[] =
                { "C", "C#", "D", "D#", "E", "F",
                  "F#", "G", "G#", "A", "A#", "B" };
        static const char* const flatNoteNames[]  =
                { "C", "Db", "D", "Eb", "E", "F",
                  "Gb", "G", "Ab", "A", "Bb", "B" };

        std::string s = sharpNoteNames[_datos[0] % 12];
        s.append(std::to_string(_datos[0] / 12 + (3 - 5)));

        return s;
    }
    [[nodiscard]] uint16_t Mensaje::formatear_pitch() const
    {
        return _datos[0]  | (_datos[1]  << 7);
    }
    [[nodiscard]] std::vector<uint8_t> Mensaje::data_bytes() const
    {
        return _datos;
    }
};