//
// Created by dario on 10/11/22.
//

#ifndef CONTROL_MIDI_COMUNICACION_HPP
#define CONTROL_MIDI_COMUNICACION_HPP

//STL:
#include <iostream>

//Qt:
#include <QObject>

//Midi:
#include <RtMidi.h>
#include "mensaje_midi.hpp"

class Comunicacion: public QObject
{
    Q_OBJECT //Macro que añade variables básicas del QObject
    public:
        Comunicacion();
        ~Comunicacion();
    public slots:
        void process();
    signals:
        void finished();
        void error(QString err);
        void print(int cod);
    private:
        // add your variables here
};


#endif //CONTROL_MIDI_COMUNICACION_HPP
