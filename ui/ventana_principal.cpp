//
// Created by dario on 9/27/22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ventana_principal.h" resolved

#include "ventana_principal.hpp"
#include "ui_ventana_principal.h"


ventana_principal::ventana_principal(QWidget *parent) :
        QMainWindow(parent) , ui(new Ui::ventana_principal)
{
    ui->setupUi(this);
}

ventana_principal::~ventana_principal()
{
    delete ui;
}
