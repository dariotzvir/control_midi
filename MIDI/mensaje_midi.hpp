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
    static constexpr std::string_view NOTE_OFF       = "Note Off        ";
    static constexpr std::string_view NOTE_ON        = "Note On         ";
    static constexpr std::string_view KEY_PRESSURE   = "Key Pressure    ";
    static constexpr std::string_view CTRL_CHANGE    = "Control Change  ";
    static constexpr std::string_view PGRM_CHANGE    = "Program Change  ";
    static constexpr std::string_view CHAN_PRESSURE  = "Channel pressure";
    static constexpr std::string_view PITCH_BEND     = "Pitch Bend      ";
    static constexpr std::string_view SYS_ESCLUSIVE  = "System exclusive";

    using status_type = std::map<const uint8_t, std::string_view>;
    /*
     * Valores específicos para los distintos tipos de comandos MIDI
     */
    static const status_type codigo_status =
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
        /*
         * Por convención las variables privadas se empiezan o terminan con "_"
         */
        const uint8_t _status;
        const std::vector<uint8_t> _datos;
        const double _dif_tiempo;

        /*
         * Estos textos de [[nodiscard]] se usan en conjunto con const para
         * optimizar la compilación.
         * Lo que hace const es que indica al programa que calcule una vez el
         * retorno para que siempre devuelva el mismo cálculo, ya que se asume
         * que el cálculo que hacemos es constante a lo largo del programa
         */
        [[nodiscard]] std::string      hex_a_nota() const;
        [[nodiscard]] uint16_t         formatear_pitch() const;
    public:
        explicit Mensaje(uint8_t status, std::vector<uint8_t> &datos, double dif_tiempo=0);
        [[nodiscard]] std::string_view status() const;
        [[nodiscard]] uint8_t          canal() const;
        [[nodiscard]] std::string      string() const;
        [[nodiscard]] std::vector<uint8_t> data_bytes() const;
    };

}


#endif //CONTROL_MIDI_MENSAJE_MIDI_HPP
