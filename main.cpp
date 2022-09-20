#include <QApplication>
#include <QPushButton>
#include <nlohmann/json.hpp>
#include <iostream>
using json = nlohmann::json;

int main(int argc, char *argv[])
{
    std::ifstream jorge("config/mkII_data.json", std::ifstream::in);
    json data = json::parse(jorge);
    std::cout << data << endl;
    QApplication a(argc, argv);
    QPushButton button("Hello world!", nullptr);
    button.resize(200, 100);
    button.show();
    return QApplication::exec();
}
