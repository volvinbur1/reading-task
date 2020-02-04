#include "config.h"
#include <QDebug>
#include "toml11-master/toml.hpp"
#include <iostream>
#include <QString>

Config::Config(QString _path) {
    try {
        const auto data = toml::parse(_path.toStdString());

        title = QString::fromStdString(toml::find<string>(data, "title"));
        path = QString::fromStdString(toml::find<string>(data, "file", "path"));
        auto temp_file_names = toml::find<vector<string>>(data, "file", "names");
        for (std::string str : temp_file_names) {
            file_names.append(QString::fromStdString(str));
        }
        speed = toml::find<float>(data, "settings", "speed");

        if (file_names.empty()) {
            qDebug() << QString::fromStdString(toml::format_error("[error] There is no files to read.", data.at("file"), "enter information about files here")) << endl;
            error = true;
            return;
        }
    }
    catch (const toml::exception& e) {
        qDebug() << e.what() << endl;
        error = true;
    }
    _speed_convert();
    _path_corect();

}

void Config::print_all() {
    qDebug() << &title << endl;
    (path == "") ? qDebug() << __FILE__ << endl : qDebug() << &path << endl;

    for (QString name : file_names) {
        qDebug() << &name << "\t";
    }

    qDebug() << endl << speed << endl;
}

void Config::_speed_convert() {
    if (speed != -1)
        speed *= 1024.f;
    if (speed < -1)
        speed = -1;
}

void Config::_path_corect() {
    if (path != "")
        path += "\\";
}

void Config::_existence_check() {

}
