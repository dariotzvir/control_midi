//
// Created by dario on 10/3/22.
//

#ifndef CONTROL_MIDI_VENTANA_PRINCIPAL_H
#define CONTROL_MIDI_VENTANA_PRINCIPAL_H

//STL:
#include <iostream>
#include <string>

//Qt:
#include <QWidget>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>

QT_BEGIN_NAMESPACE //Genera solo CLion
namespace Ui
{
    class ventana_principal;
}
QT_END_NAMESPACE

class ventana_principal : public QWidget
{
Q_OBJECT

public:

    explicit ventana_principal(QWidget *parent = nullptr);
    ~ventana_principal() override;

    //Terminal:
    void limpiar_terminal();
    void agregar_texto_terminal(const std::string &texto);
    void agregar_texto_terminal(const QString &texto);
    void agregar_texto_terminal(const std::string &texto, const QColor &color);
    void agregar_texto_terminal(const QString &texto, const QColor &color);
    void mensaje_conectando_terminal();

    //Temas:

    //Barra de estado:
    void dispositivo_conectado   (const QString &nombre);
    void dispositivo_desconectado(const QString &nombre);
    void cambiando_configuracion (const QString &nombre);

    //ComboBox:
    void llenar_colores();
    void desactivar_lista_colores();

public slots:
    void print(int cod);

private:
    Ui::ventana_principal *ui;
};


#endif //CONTROL_MIDI_VENTANA_PRINCIPAL_H
