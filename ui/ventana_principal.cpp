//
// Created by dario on 10/3/22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ventana_principal.h" resolved


#include "ventana_principal.h"
#include "ui_ventana_principal.h"


ventana_principal::ventana_principal(QWidget *parent):
QWidget(parent), ui(new Ui::ventana_principal)
{
    hilo = nullptr;
    proceso_comunicacion = nullptr;
    ui->setupUi(this);

    connect(ui->actionLimpiar_consola, &QAction::triggered,
            this, &ventana_principal::limpiar_terminal);
    connect(ui->action_consola, &QAction::triggered,
            this, &ventana_principal::activar_terminal);
    connect(ui->aplicar_cambios, &QPushButton::clicked,
            this, &ventana_principal::validar_comando);
    connect(ui->actionRefrescar_puertos, &QAction::triggered,
            this, &ventana_principal::buscar_puertos);
    connect(ui->boton_reducir, &QPushButton::pressed,
            this, &ventana_principal::achicar_fuente);
    connect(ui->boton_agrandar, &QPushButton::pressed,
            this, &ventana_principal::agrandar_fuente);

    activar_lista_colores(false);
    ui->action_consola->setChecked(true);
    ui->menuPuertos->setEnabled(false);
    ui->menuPuertos->setTearOffEnabled(false);

    auto *atajo_mas = new QShortcut(QKeySequence(tr("Ctrl++", "Agrandar texto")),
                                 this);
    auto *atajo_menos = new QShortcut(QKeySequence(tr("Ctrl+-", "Achicar texto")),
                                 this);
    connect(atajo_mas, &QShortcut::activated,
            this, &ventana_principal::agrandar_fuente);
    connect(atajo_menos, &QShortcut::activated,
            this, &ventana_principal::achicar_fuente);
}
ventana_principal::~ventana_principal()
{
    delete ui;
    delete hilo;
    delete proceso_comunicacion;
}
void ventana_principal::recibir_msj_midi(Formatear_Midi::Mensaje msj)
{
    QString str_msj = msj.string().c_str();
    agregar_texto_terminal(str_msj);
}
void ventana_principal::limpiar_terminal()
{
    ui->terminal->clear();
    ui->terminal->clearFocus();
}
void ventana_principal::activar_terminal(bool flag)
{
    ui->terminal->setEnabled(flag);
    limpiar_terminal();
}
void ventana_principal::validar_comando(int index_lista)
{
    QString opcion_seleccionada = ui->selector_color->currentText();
    if(opcion_seleccionada.isEmpty())
    {
        mensaje_barra_estado("Nada seleccionado");
        return;
    }
    if(!hilo->isRunning())
    {
        mensaje_barra_estado("No hay dispositivo conectado");
        return;
    }
    cambiando_configuracion("Cambiando luces a " + opcion_seleccionada);
    uint param_midi = ui->selector_color->currentData().toUInt();
    uint memoria = ui->selector_memoria->value();

    std::vector<uint8_t> data_bytes = {0xF0, 0x7E, 0x7F, 0x06, uint8_t(memoria), 0xF7};
    auto iniciar_cmd = Formatear_Midi::Mensaje(0xF0, data_bytes);
    proceso_comunicacion->enviar_comando(iniciar_cmd);

    for(int id_pad: _lista_pads)
    {
        std::vector<uint8_t> data_bytes =
        {
            0xF0, 0x00, 0x20, 0x6B, 0x7F, 0x42, 0x02, 0x00,
            uint8_t(_id_colores), uint8_t(id_pad), uint8_t(param_midi), 0xF7
        };

        auto cmd = Formatear_Midi::Mensaje(0xF0, data_bytes);
        proceso_comunicacion->enviar_comando(cmd);
    }
    data_bytes = {0xF0, 0x00, 0x20, 0x6B, 0x7F, 0x42, 0x02, 0x00, 0x40, 0x1E, 0x7F, 0xF7};
    auto refrescar_luces = Formatear_Midi::Mensaje(0xF0, data_bytes);
    proceso_comunicacion->enviar_comando(refrescar_luces);

    data_bytes = {0xF0, 0x00, 0x20, 0x6B, 0x7F, 0x42, 0x06, uint8_t(memoria), 0xF7};
    auto terminar_cmd = Formatear_Midi::Mensaje(0xF0, data_bytes);
    proceso_comunicacion->enviar_comando(terminar_cmd);
}
void ventana_principal::agrandar_fuente()
{
    cambiar_fuente(1);
};
void ventana_principal::achicar_fuente()
{
    cambiar_fuente(-1);
};
void ventana_principal::cambiar_fuente(int cambio)
{
    QFont fuente = ui->terminal->font();
    QFont copia = QFont(fuente);
    int tamaño_actual = copia.pointSize();
    if(tamaño_actual < 50 && cambio > 0) copia.setPointSize(copia.pointSize() + cambio);
    if(tamaño_actual > 5  && cambio < 0) copia.setPointSize(copia.pointSize() + cambio);

    ui->terminal->setFont(copia);
};
void ventana_principal::agregar_texto_terminal(const QString &texto)
{
    agregar_texto_terminal(texto, QColor(255, 255, 255));
}
void ventana_principal::agregar_texto_terminal(const QString &texto, const QColor &color)
{
    if(!ui->terminal->isEnabled()) return;
    QString str_html = "<div style=" + color.name() +
                        ";>" + texto.toHtmlEscaped() + "</div>";
    ui->terminal->appendHtml(str_html);
}
void ventana_principal::mensaje_conectando_terminal(const QString &nombre)
{
    const QString mensaje_conexion = "Conectado a: " + nombre + "...";
    agregar_texto_terminal(mensaje_conexion);
}
void ventana_principal::llenar_colores(int id, std::vector<std::pair<std::string, int>> lista_colores)
{
    _id_colores = id;

    for(auto[nombre, param]: lista_colores)
        ui->selector_color->addItem(nombre.c_str(), param);
}
void ventana_principal::activar_lista_colores(bool flag)
{
    ui->selector_color->setEnabled(flag);
    if(!flag) ui->selector_color->setPlaceholderText("Conecte un dispositivo");
    else ui->selector_color->setPlaceholderText("Seleccione un color");
}
void ventana_principal::llenar_pads(const std::vector<int> &pads)
{
    _lista_pads = pads;
}
void ventana_principal::dispositivo_conectado(const QString &nombre)
{
    const QString msj_conexion = "Dispositivo: " + nombre + " conectado";
    mensaje_barra_estado(msj_conexion);
}
void ventana_principal::dispositivo_desconectado()
{
    const QString msj_desconexion = "Dispositivo desconectado";
    mensaje_barra_estado(msj_desconexion);
}
void ventana_principal::dispositivo_fallo_con(const QString &nombre)
{
    const QString msj_fallo = "Dispositivo: " + nombre + " fallo al conectar";
    mensaje_barra_estado(msj_fallo);
}
void ventana_principal::cambiando_configuracion(const QString &nombre)
{
    const QString msj = "Configurando: " + nombre;
    mensaje_barra_estado(msj);
}
void ventana_principal::mensaje_barra_estado(const QString msj)
{
    ui->statusbar->showMessage(msj, 5000);
}
void ventana_principal::levantar_hilo(uint puerto)
{
    std::cout << "conectando a: " << puerto << "\n";
    if(hilo != nullptr && hilo->isRunning())
    {
        proceso_comunicacion->finalizar();
        hilo->exit();
    }
    hilo = new QThread();
    proceso_comunicacion = new Comunicacion(puerto);
    proceso_comunicacion->moveToThread(hilo);
    connect(hilo, &QThread::started,
            proceso_comunicacion, &Comunicacion::process);
    connect(proceso_comunicacion, &Comunicacion::finished,
            hilo, &QThread::quit);
    connect(proceso_comunicacion, &Comunicacion::finished,
            proceso_comunicacion, &Comunicacion::deleteLater);
    connect(hilo, &QThread::finished,
            hilo, &QThread::deleteLater);
    connect(proceso_comunicacion, &Comunicacion::error,
            this, &ventana_principal::mensaje_barra_estado);
    connect(proceso_comunicacion, &Comunicacion::msj_midi,
            this, &ventana_principal::recibir_msj_midi);
    connect(proceso_comunicacion, &Comunicacion::finished,
            this, &ventana_principal::dispositivo_desconectado);
    hilo->start();
}
void ventana_principal::buscar_puertos()
{
    ui->menuPuertos->clear();
    ui->menuPuertos->setEnabled(true);
    ui->menuPuertos->setTearOffEnabled(true);
    _lista_puertos = listar_puertos();
    for(const Dispositivo &disp: _lista_puertos)
    {
        QAction *accion = new QAction(ui->menuPuertos);
        accion->setText(disp.nombre.c_str());
        accion->setData(disp.puerto);
        ui->menuPuertos->addAction(accion);
    }

    connect(ui->menuPuertos, &QMenu::triggered,
            this, &ventana_principal::conectar_puerto);
}
void ventana_principal::conectar_puerto(QAction *accion)
{
    const QString nombre = accion->text();
    const uint puerto = accion->data().toUInt(); //Esto devuelve un QVariant
    limpiar_terminal();
    mensaje_conectando_terminal(nombre);
    dispositivo_conectado(nombre);
    levantar_hilo(puerto);
    activar_lista_colores(true);

}

void ventana_principal::tema_claro ()
{
    this->setStyle(QStyleFactory::create("adwaita"));
}
void ventana_principal::tema_oscuro()
{
    this->setStyle(QStyleFactory::create("adwaita-dark"));
}
