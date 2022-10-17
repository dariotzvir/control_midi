//
// Created by dario on 10/11/22.
//

#include "comunicacion.hpp"

Comunicacion::Comunicacion(uint n_puerto): _puerto(n_puerto)
{
    continuar_proceso = true;
}
void Comunicacion::process()
{
    std::unique_ptr<RtMidiIn>  in (new RtMidiIn());   //RtMidiOut *in  = new RtMidiIn();
    std::unique_ptr<RtMidiOut> out(new RtMidiOut()); //RtMidiOut *out = new RtMidiOut();
    uint puertos_in  = in->getPortCount();
    uint puertos_out = out->getPortCount();
    in->getPortName();

    if(puertos_in < _puerto || puertos_out < _puerto)
    {
        emit error("No se encontró el puerto deseado");
        emit finished();
        return;
    }

    in->openPort(_puerto);
    out->openPort(_puerto);

    if(!in->isPortOpen() || !out->isPortOpen())
    {
        emit error("No se pudo abrir el puerto");
        emit finished();
        return;
    }

    in->ignoreTypes(false, false, false);

    while(continuar_proceso)
    {
        std::vector<uint8_t> mensaje;
        double tiempo;
        try
        {
            tiempo = in->getMessage(&mensaje);
        }
        catch(RtMidiError &codigo_error)
        {
            emit error("Se perdió conexión con el dispositivo");
            break;
        }

        unsigned long n_bytes = mensaje.size();
        if(n_bytes >= 3)
        {
            uint8_t status = mensaje[0];
            std::reverse(mensaje.begin(), mensaje.end());
            mensaje.pop_back();
            std::reverse(mensaje.begin(), mensaje.end());
            auto msj = Formatear_Midi::Mensaje(status, mensaje, tiempo);
            emit msj_midi(msj);
        }
        if(!_cola_ingreso.empty())
        {
            Formatear_Midi::Mensaje msj = _cola_ingreso.front();
            _cola_ingreso.pop();
            std::vector<uint8_t> aux =  msj.data_bytes();
            out->sendMessage(&aux);
        }
        if(!in->isPortOpen() || !out->isPortOpen())
        {
            emit error("Se perdió conexión con el dispositivo");
            break;
        }
    }

    in->closePort();
    out->closePort();

    emit finished();
}

void Comunicacion::enviar_comando(Formatear_Midi::Mensaje msj)
{
    _cola_ingreso.push(msj);
}

void Comunicacion::finalizar()
{
    continuar_proceso = false;
}

std::vector<Dispositivo> listar_puertos()
{
    std::unique_ptr<RtMidiIn>  in (new RtMidiIn());
    uint n = in->getPortCount();
    std::vector<Dispositivo> lista;
    for(uint i=0; i<n; i++)
    {
        auto nombre = in->getPortName(i);
        nombre = nombre.substr(0, nombre.find(':'));
        lista.push_back(Dispositivo(i,nombre));
    }



    return lista;
}