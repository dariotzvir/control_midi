//C++ STL (Standard Template Library):
#include <bits/stdc++.h>
using namespace std;

//Qt:
#include <QApplication>
#include <QMainWindow>
#include <QStyle>
#include <QString>
#include <QMenuBar>
#include <QMenu>
#include <QList>
#include <QThread>

//Boost:
#include <boost/json.hpp>
namespace json = boost::json;

//UI:
#include "ui/ventana_principal.h"

//MIDI:
#include "MIDI/mensaje_midi.hpp"
#include "MIDI/comunicacion.hpp"

string cargar_archivo_stream()
{
    const string RUTA_ARCHIVO_CONFIG = "../config/MiniLabMK2.json";
    ifstream in(RUTA_ARCHIVO_CONFIG);
    if(!in.is_open()) return "";
    ostringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

pair <json::object, json::error_code> leer_json()
{
    string str_archivo = cargar_archivo_stream();

    json::error_code ec;
    const json::parse_options opt =
    {
        .max_depth             = 32,
        .allow_comments        = true,
        .allow_trailing_commas = true,
        .allow_invalid_utf8    = true
    };

    json::storage_ptr ptr_aux;
    json::value json_parseado = json::parse(str_archivo, ec, ptr_aux, opt);
    if(ec) return {json::object(), ec};
    return {json_parseado.as_object(), ec};
}

int main(int argc, char *argv[])
{
    uint8_t status = 0xEF;
    vector<uint8_t> datos =  {0x7F, 0x7F};
    auto mensaje = Formatear_Midi::Mensaje(status, datos);

    cout << mensaje.string() << endl;
    auto [json, error] = leer_json();
    cout << json << endl << error << endl;
    cout << json["name"] << " " << json["nam"] << endl;

    QApplication app(argc , argv);
    app.setApplicationName("Control Midi");
    app.setApplicationDisplayName("Control Midi");
    ventana_principal ventana;

    QThread* hilo_comunicacion = new QThread();
    Comunicacion* proceso_comunicacion = new Comunicacion();
    proceso_comunicacion->moveToThread(hilo_comunicacion);
    ventana.connect(hilo_comunicacion, &QThread::started, proceso_comunicacion, &Comunicacion::process);
    ventana.connect(proceso_comunicacion, &Comunicacion::print, &ventana, &ventana_principal::print);
    ventana.connect(proceso_comunicacion, &Comunicacion::finished, hilo_comunicacion, &QThread::quit);
    ventana.connect(proceso_comunicacion, &Comunicacion::finished, proceso_comunicacion, &Comunicacion::deleteLater);
    ventana.connect(hilo_comunicacion, &QThread::finished, hilo_comunicacion, &QThread::deleteLater);
    hilo_comunicacion->start();

    ventana.show();
    ventana.agregar_texto_terminal(mensaje.string());
    ventana.agregar_texto_terminal(mensaje.string(), QColor(23,23,23));
    ventana.dispositivo_conectado("jose luis");



    return app.exec();
}
