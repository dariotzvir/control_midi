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
#include <QPalette>

//Boost:
#include <boost/json.hpp>
namespace json = boost::json;

//UI:
#include "ui/ventana_principal.h"

//MIDI:
#include "MIDI/mensaje_midi.hpp"
#include "MIDI/comunicacion.hpp"
#include <RtMidi.h> //Librería específica para hacer la comunicación MIDI

/*
 * Esta función escribe elm archivo de configuración a un string
 */
string cargar_archivo_stream()
{
    const string RUTA_ARCHIVO_CONFIG = "../config/MiniLabMK2.json";
    ifstream in(RUTA_ARCHIVO_CONFIG);
    if(!in.is_open()) return ""; //Condición de falla
    ostringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

/*
 * Esta función carga el archivo de configuración con la función anterior y lo
 * formatea en un objeto JSON de la librería de boost
 */
pair <json::object, json::error_code> leer_archivo_json()
{
    string str_archivo = cargar_archivo_stream();

    json::error_code ec;
    //Configuraciones que usaremeos para leer el JSON
    const json::parse_options opt =
    {
        .max_depth             = 32,
        .allow_comments        = true,
        .allow_trailing_commas = true,
        .allow_invalid_utf8    = true
    };

    json::storage_ptr ptr_aux;
    json::value json_parseado = json::parse(str_archivo, ec, ptr_aux, opt);
    if(ec) return {json::object(), ec}; //Verifica que no sea un error
    return {json_parseado.as_object(), ec}; //Retorna como un pair tanto el error como el JSON
}

/*
 * Dado una lista de JSON retorna uno de los campos en función a un ID numérico
 */
json::object buscar_field(json::array &lista_campos, const int campo)
{
    json::object retorno;
    /*
     * Este for es un for in range o auto, es equivalente a lo siguiente
     * for(int i=0; i<lista_campos.lenght(); i++)
     * json::value item = lista_campos[i];
     * ...
     */
    for(json::value item: lista_campos)
    {
        if(!item.is_object()) continue;
        json::object &obj = item.as_object();

        if(!obj.contains("id")) continue;
        json::value obj_id = obj.at("id");

        if(!obj_id.is_int64()) continue;
        int id = obj_id.as_int64();

        if(id == campo)
        {
            retorno = obj;
            break;
        }
    }
    return retorno;
}
/*
 * Busca un campo de "pad" en un a lista determinada
 */
vector<int> buscar_id_pads(json::array &lista_campos)
{
    vector<int> retorno;
    string campo = "pad";
    for(json::value item: lista_campos)
    {
        if(!item.is_object()) continue;
        json::object &obj = item.as_object();

        if(!obj.contains("id")) continue;
        json::value obj_id = obj.at("id");

        if(!obj_id.is_string()) continue;
        string id = obj_id.as_string().c_str();

        if(id.substr(0, id.size()-1) == campo)
        {
            json::array aux = obj.at("items").as_array();
            for(json::value i: aux)
                retorno.push_back(i.at("id").as_int64());

        }
    }
    return retorno;
}

int main(int argc, char *argv[])
{
    /*
     * Esto hace un poco de magia para poder leer los campos específicos del
     * archivo de configuración, ver Boost::Json
     */
    auto [json, error]        = leer_archivo_json();
    json::array campos        = json["fields"].as_array();
    json::array controles     = json["controls"].as_array();
    json::object colores      = buscar_field(campos, 50);
    vector<int> lista_id_pads = buscar_id_pads(controles);

    int param_midi = colores.at("paramId").as_int64();
    vector<pair<string, int>> lista_colores;
    for(json::value i: colores["list"].as_array())
    {
        string color = i.at("str").as_string().c_str();
        int param_midi = i.at("val").as_int64();

        lista_colores.push_back({color, param_midi});
    }
    QApplication app(argc , argv);
    app.setApplicationName("Control Midi");
    app.setApplicationDisplayName("Control Midi");
    ventana_principal ventana;
    ventana.llenar_pads(lista_id_pads);
    ventana.llenar_colores(param_midi, lista_colores);
    ventana.show();
    return app.exec(); //Este método entrega el control del programa a Qt para que ejecute los eventos
}
