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

//Boost:
#include <boost/json.hpp>
namespace json = boost::json;

//UI:
#include "ui/ventana_principal.h"

//MIDI:
#include "MIDI/mensaje_midi.hpp"

string RUTA_ARCHIVO_CONFIG = "../config/MiniLabMK2.json";

std::string slurp(std::ifstream& in) {
    std::ostringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

void stream_ads()
{
    ifstream archivo_config;
    archivo_config.open(RUTA_ARCHIVO_CONFIG);
    string archivo = slurp(archivo_config);
    json::error_code ec;
    json::parse_options opt;
    opt.allow_trailing_commas = true;
    opt.allow_invalid_utf8    = true;
    opt.allow_comments = true;

    json::storage_ptr asd;
    json::value a = json::parse(archivo, ec,asd , opt);
    asd.is_deallocate_trivial();
    cout << "Json: " << endl << a << endl;
    cout << "Error: " << ec.message() << endl;
}
//
//void cargar_json()
//{
//    ifstream archivo_config;
//    archivo_config.open(RUTA_ARCHIVO_CONFIG);
//    cout << archivo_config.is_open() << endl;
//    string str_config = slurp(archivo_config);
//    cout << str_config;
//    archivo_config.close();
//    json::parse_options opt;
//    opt.allow_comments        = true;
//    opt.allow_invalid_utf8    = true;
//    opt.allow_trailing_commas = true;
//    json::object config;
//    json::value a = boost::json::parse(str_config, opt);
//    cout << a << endl;
//}

int main(int argc, char *argv[])
{
    vector<uint8_t> aux =  {0x7F, 0x7F};
    auto a = Formatear_Midi::Mensaje(0xEF, aux);
    cout << a.string() << endl;
    stream_ads();

    QApplication app(argc , argv);
    app.setApplicationName("Control Midi");
    app.setApplicationDisplayName("Control Midi");
    ventana_principal ventana;
    ventana.show();

    return app.exec();
}
