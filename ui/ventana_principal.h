//
// Created by dario on 10/3/22.
//

#ifndef CONTROL_MIDI_VENTANA_PRINCIPAL_H
#define CONTROL_MIDI_VENTANA_PRINCIPAL_H

//STL:
#include <iostream>
#include <string>
#include <queue>

//Qt:
#include <QWidget>
#include <QString>
#include <QThread>
#include <QShortcut>
#include <QStyleFactory>

//Midi:
#include <RtMidi.h>
#include "mensaje_midi.hpp"
#include "comunicacion.hpp"

/*
 * Este namespace lo genera solo Qt Creator para poder luego vincular lo que se
 * compila desde el designer.
 */
QT_BEGIN_NAMESPACE
namespace Ui
{
    class ventana_principal;
}
QT_END_NAMESPACE

class ventana_principal : public QWidget
{
Q_OBJECT
    Ui::ventana_principal *ui; //Esto se genera solo para vincular lo compilado en UI
    std::vector<Dispositivo> _lista_puertos;
    QThread *hilo;
    Comunicacion *proceso_comunicacion;
    std::vector<int> _lista_pads;
    int _id_colores;
public:
    explicit ventana_principal(QWidget *parent = nullptr);
    ~ventana_principal        () override;

    //Terminal:
public slots:
    void recibir_msj_midi(Formatear_Midi::Mensaje msj);
    void limpiar_terminal();
    void activar_terminal(bool flag);
    void mandar_comando (int index_lista);
    void agrandar_fuente();
    void achicar_fuente();
public:
    void cambiar_fuente(int cambio);
    void agregar_texto_terminal     (const QString &texto);
    void agregar_texto_terminal     (const QString &texto, const QColor &color);
    void mensaje_conectando_terminal(const QString &nombre);

    //Barra de estado:
    void dispositivo_conectado   (const QString &nombre);
    void dispositivo_desconectado();
    void cambiando_configuracion (const QString &nombre);
    void mensaje_barra_estado    (const QString msj);

    //ComboBox:
    /*
     * ComboBox es la lista desplegable desde la que se seleccionan los colores
     */
    void llenar_colores(int id, std::vector<std::pair<std::string, int>> lista_colores);
    void activar_lista_colores(bool flag);
    void llenar_pads(const std::vector<int> &pads);

    //Puertos:
    void levantar_hilo  (uint puerto); //Esta función creará el hilo de comunicación
public slots:
    void buscar_puertos ();
    void conectar_puerto(QAction *accion);

};


#endif //CONTROL_MIDI_VENTANA_PRINCIPAL_H
