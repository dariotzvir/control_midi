//
// Created by dario on 10/3/22.
//

#ifndef CONTROL_MIDI_VENTANA_PRINCIPAL_H
#define CONTROL_MIDI_VENTANA_PRINCIPAL_H

//Qt:
#include <QWidget>
#include <QString>



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
    void agregar_texto_terminal(const QString &texto);
    void agregar_texto_terminal(const QString &texto, const QColor &color);
    void mensaje_conectando_terminal();

    //Temas:

    //

private:
    Ui::ventana_principal *ui;
};


#endif //CONTROL_MIDI_VENTANA_PRINCIPAL_H
