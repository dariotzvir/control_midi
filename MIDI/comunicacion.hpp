//
// Created by dario on 10/11/22.
//

#ifndef CONTROL_MIDI_COMUNICACION_HPP
#define CONTROL_MIDI_COMUNICACION_HPP

//STL:
#include <iostream>
#include <memory>
#include <vector>
#include <queue>

#include <memory>

//Qt:
#include <QObject>

//Midi:
#include <RtMidi.h>
#include "mensaje_midi.hpp"

class Comunicacion: public QObject
{
    Q_OBJECT //Macro que añade variables básicas del QObject
    bool continuar_proceso;
    const uint _puerto;
    std::queue<Formatear_Midi::Mensaje> _cola_ingreso;

public:
    explicit Comunicacion(uint n_puerto);
public slots:
    void process();
    void enviar_comando(Formatear_Midi::Mensaje msj);
    void finalizar();
signals:
    void finished();
    void error(const QString err);
    void msj_midi(Formatear_Midi::Mensaje msj);
};

class Dispositivo
{
public:
    const uint puerto;
    const std::string nombre;
    Dispositivo(uint puerto_arg, std::string nombre_arg):
    puerto(puerto_arg), nombre(nombre_arg)
    {};
};

std::vector<Dispositivo> listar_puertos();


#endif //CONTROL_MIDI_COMUNICACION_HPP
