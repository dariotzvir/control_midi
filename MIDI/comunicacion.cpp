//
// Created by dario on 10/11/22.
//

#include "comunicacion.hpp"

Comunicacion::Comunicacion()
{

}
Comunicacion::~Comunicacion()
{

}
void Comunicacion::process()
{
    for(int i = 0; i < 50; i++)
        emit print(i);

    emit finished();
}
