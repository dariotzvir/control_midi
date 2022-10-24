//
// Created by dario on 10/11/22.
//

#include "comunicacion.hpp"

//Señal para terminar la ejecución
Comunicacion::Comunicacion(uint n_puerto): _puerto(n_puerto)
{
    continuar_proceso = true;
}
void Comunicacion::process()
{
    /*
     * unique_ptr permite borrar automáticamente memoria dinámica cuando se sale
     * del bloque en que se declaro
     */
    std::unique_ptr<RtMidiIn>  in (new RtMidiIn());  //RtMidiOut *in  = new RtMidiIn();
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

    //El while solo se corta cuando el hilo principal llama a la señal que corta el proceso
    while(continuar_proceso)
    {
        std::vector<uint8_t> mensaje;
        double tiempo;
        /*
         * try funciona de la misma manera que en Python donde previene una
         * excepción que pararía el programa
         */
        try
        {
            tiempo = in->getMessage(&mensaje);
        }
        catch(RtMidiError &codigo_error)
        {
            emit error("Se perdió conexión con el dispositivo");
            break;
        }

        /*
         * Lee el mensaje y si es válido (que tenga más de 3 bytes) lo procesa
         * mandándolo al hilo principal
         */
        unsigned long n_bytes = mensaje.size();
        if(n_bytes >= 3)
        {
            uint8_t status = mensaje[0];
            /*
             * Esto es un poco de magia para poder obtener y eliminar el primer
             * elemento del vector
             */
            std::reverse(mensaje.begin(), mensaje.end());
            mensaje.pop_back();
            std::reverse(mensaje.begin(), mensaje.end());
            auto msj = Formatear_Midi::Mensaje(status, mensaje, tiempo);
            emit msj_midi(msj); //Manda al hilo principal
        }
        /*
         * Verifica que la cola de ingreso a donde llegan los mensajes del
         * hilo principal este vacía, si tiene un dato lo popea y manda al
         * dispositivo
         */
        if(!_cola_ingreso.empty())
        {
            Formatear_Midi::Mensaje msj = _cola_ingreso.front();
            _cola_ingreso.pop();
            std::vector<uint8_t> aux = msj.data_bytes();
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

/*
 * La función es un slot que recibe los mensajes desde el hilo principal y lo
 * empuja a la lista para que luego el loop la lea
 */
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