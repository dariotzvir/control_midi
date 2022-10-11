//
// Created by dario on 10/3/22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ventana_principal.h" resolved

#include "ventana_principal.h"
#include "ui_ventana_principal.h"


ventana_principal::ventana_principal(QWidget *parent) :
        QWidget(parent), ui(new Ui::ventana_principal)
{
    ui->setupUi(this);
}

ventana_principal::~ventana_principal()
{
    delete ui;
}
void ventana_principal::limpiar_terminal()
{
    ui->terminal->clear();
    ui->terminal->clearFocus();
}
void ventana_principal::agregar_texto_terminal(const std::string &texto)
{
    agregar_texto_terminal(QString(texto.c_str()));
}
void ventana_principal::agregar_texto_terminal(const QString &texto)
{
    agregar_texto_terminal(texto, QColor(255, 255, 255));
}
void ventana_principal::agregar_texto_terminal(const std::string &texto, const QColor &color)
{
    agregar_texto_terminal(QString(texto.c_str()), color);
}
void ventana_principal::agregar_texto_terminal(const QString &texto, const QColor &color)
{
    QString str_html = "<div style=" + color.name() + ";>" + texto.toHtmlEscaped() + "</div>";
    ui->terminal->appendHtml(str_html);
}
void ventana_principal::mensaje_conectando_terminal()
{
    const QString mensaje_conexion = "Conectado...";
    agregar_texto_terminal(mensaje_conexion);
}
void ventana_principal::llenar_colores()
{

}
void ventana_principal::desactivar_lista_colores()
{

}

void ventana_principal::dispositivo_conectado(const QString &nombre)
{
    const QString msj_conexion = "Dispositivo: " + nombre + " conectado";
    ui->statusbar->showMessage(msj_conexion, 5000);
}
void ventana_principal::dispositivo_desconectado(const QString &nombre)
{
    const QString msj_desconexion = "Dispositivo: " + nombre + " desconectado";
    ui->statusbar->showMessage(msj_desconexion, 5000);
}
void ventana_principal::cambiando_configuracion(const QString &nombre)
{
    const QString msj = "Configurando: " + nombre;
    ui->statusbar->showMessage(msj, 5);
}

void ventana_principal::print(int cod)
{
    std::cout << "Codigod desde señal: "  << cod << std::endl;
}

