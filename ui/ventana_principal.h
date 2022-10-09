//
// Created by dario on 10/3/22.
//

#ifndef CONTROL_MIDI_VENTANA_PRINCIPAL_H
#define CONTROL_MIDI_VENTANA_PRINCIPAL_H

#include <QWidget>


QT_BEGIN_NAMESPACE
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

private:
    Ui::ventana_principal *ui;
};


#endif //CONTROL_MIDI_VENTANA_PRINCIPAL_H
