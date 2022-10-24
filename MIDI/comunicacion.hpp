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

/*
 * Esta clase hereda de QObject para tener las utilidades de las señales de Qt,
 * también para poder crear el hilo de comunicación
 */
class Comunicacion: public QObject
{
    Q_OBJECT //Macro que añade variables básicas del QObject
    bool continuar_proceso; //Flag global para el proceso
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

/*
 * Esta clase se usa para listar los dispositivos que aparecen cuando apretás
 * refrescar puertos
 */
class Dispositivo
{
public:
    const uint puerto;
    const std::string nombre;
    Dispositivo(uint puerto_arg, std::string nombre_arg):
    puerto(puerto_arg), nombre(nombre_arg)
    {};
};
//Función extra para obtener los dispositivos conectados actualmente
std::vector<Dispositivo> listar_puertos();

#endif //CONTROL_MIDI_COMUNICACION_HPP
