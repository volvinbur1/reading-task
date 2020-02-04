#pragma once

#include <QString>
#include <QVector>
using namespace std;

class Config {
public:
    Config(QString _path);
    void print_all();
    QString title;
    QString path;
    QVector<QString> file_names;
    float speed;
    int amount_of_files;
    bool error = false; // true if there is some troubles with config files
private:
    void _speed_convert();
    void _path_corect();
    void _existence_check();
};
