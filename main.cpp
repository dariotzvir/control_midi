#include <iostream>

#include <QApplication>
#include <QMainWindow>
#include <QStyle>
#include <QString>

#include "ui/ventana_principal.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ventana_principal ventana;
    ventana.show();
   /* Ventana_principal aventana;
    auto hijos = aventana.children();
    for(auto i: hijos)
    {
        QString a = i->objectName();
        cout << a.toStdString() << endl;
        auto hijos_2 = i->children();
        for(auto j: hijos_2)
        {
            QString b = i->objectName();
            cout << b.toStdString() << endl;
        }
    }
    aventana.show();*/
    return app.exec();
}
