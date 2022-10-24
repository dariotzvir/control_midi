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
    ui->setupUi(this); //Generado por Qt para que arranque la UI

    /*
     * Se vinculan todas las señales que se necesitaran para la ejecución
     */
    connect(ui->actionLimpiar_consola, &QAction::triggered,
            this, &ventana_principal::limpiar_terminal);
    connect(ui->action_consola, &QAction::triggered,
            this, &ventana_principal::activar_terminal);
    connect(ui->aplicar_cambios, &QPushButton::clicked,
            this, &ventana_principal::mandar_comando);
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

    /*
     * Crea los atajos de teclado y los vincula a la función que agranda al texto
     */
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
/*
 * Recibe desde el hilo los mensajes midi del teclado
 */
void ventana_principal::recibir_msj_midi(Formatear_Midi::Mensaje msj)
{
    QString str_msj = msj.string().c_str();
    agregar_texto_terminal(str_msj);
}
void ventana_principal::limpiar_terminal()
{
    ui->terminal->clear(); //Esto borra el texto
    ui->terminal->clearFocus();  //Esto borra el cursor de la terminal
}
void ventana_principal::activar_terminal(bool flag)
{
    ui->terminal->setEnabled(flag);
    limpiar_terminal();
}
/*
 * Esta función manda los comandos de cambiar el color al hilo para que
 * luego lleguen al teclado, todos los números hexadecimales (los que empiezan
 * con 0x...) son variables propias del teclado para que funcionen
 */
void ventana_principal::mandar_comando(int index_lista)
{
    //Obtiene desde la lista desplegable lo que está seleccionado actualmente
    QString opcion_seleccionada = ui->selector_color->currentText();
    if(opcion_seleccionada.isEmpty()) //Si es inválida retorna
    {
        mensaje_barra_estado("Nada seleccionado");
        return;
    }
    /*
     * Verifica que el hilo esté corriendo
     */
    if(!hilo->isRunning())
    {
        mensaje_barra_estado("No hay dispositivo conectado");
        return;
    }

    //Esto coloca en la barra de estado inferior el siguiente mensaje
    cambiando_configuracion("Cambiando luces a " + opcion_seleccionada);
    uint param_midi = ui->selector_color->currentData().toUInt();
    uint memoria = ui->selector_memoria->value();

    //Este mensaje indica al teclado de que se va a cambiar una de sus memorias
    std::vector<uint8_t> data_bytes = {0xF0, 0x7E, 0x7F, 0x06, uint8_t(memoria), 0xF7};
    auto iniciar_cmd = Formatear_Midi::Mensaje(0xF0, data_bytes);
    proceso_comunicacion->enviar_comando(iniciar_cmd);

    //Manda para todos los pads de los colores el mismo color al hilo
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
    //Este mensaje es para que termine la configuración
    data_bytes = {0xF0, 0x00, 0x20, 0x6B, 0x7F, 0x42, 0x02, 0x00, 0x40, 0x1E, 0x7F, 0xF7};
    auto refrescar_luces = Formatear_Midi::Mensaje(0xF0, data_bytes);
    proceso_comunicacion->enviar_comando(refrescar_luces);

    //Este cambia a la memoria que acabamos de configurar
    data_bytes = {0xF0, 0x00, 0x20, 0x6B, 0x7F, 0x42, 0x06, uint8_t(memoria), 0xF7};
    auto terminar_cmd = Formatear_Midi::Mensaje(0xF0, data_bytes);
    proceso_comunicacion->enviar_comando(terminar_cmd);
}
/*
 * Estas tres funciones cambian el tamaño de la fuente, la última primero ve la
 * fuente actual que tiene la terminal, verifica que no sea muy grande o muy chica
 * y la vuelve a settear
 */
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
/*
 * Al formatear el texto como HTML la terminal peuede mostrar texto con color,
 * está la opción pero solo se usa el blanco en este caso
 */
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
    //Esta función marca al programa que ese objeto correrá en otro hilo
    proceso_comunicacion->moveToThread(hilo);
    /*
     * Conecta varias señales para que el hilo funcione.
     * Las más importantes son las de started, que marca que cuando el hilo
     * para que se ejecute el loop del objeto
     * y finished que marca al hilo que el objeto terminó su loop
     * para que el hilo se pare y no consuma más recursos.
     */
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
