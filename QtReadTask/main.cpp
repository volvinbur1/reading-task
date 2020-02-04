#include <QCoreApplication>
#include <QDebug>
#include "config.h"
#include <memory>
#include <thread>
#include <cstdio>
#include "global_var.h"

//#pragma warning(disable : 4996)
using namespace std;

bool reading_is_finished = false;
quint64 number_of_all_bytes = 0;
QList<QString> content = *new QList<QString>();
bool file_read = false;
QString error;

void read_file_content(Config* config) {
    for (QString name : config->file_names) {
        content.append("__" + name + "__");

        {
            FILE* file;// = fopen((config->path + name).toLocal8Bit().data(), "rb");
            fopen_s(&file,(config->path + name).toLocal8Bit().data(), "rb");
            auto stream = unique_ptr<FILE, decltype(&fclose)>(file, &fclose);

            if (stream != nullptr) {
                fseek(stream.get(), 0, SEEK_END);
                long size = ftell(stream.get());
                rewind(stream.get());

                //qDebug() << size << endl;

                clock_t start = clock();
                uint16_t read_bytes = 0;

                while (ftell(stream.get()) < size) {
                    if (read_bytes <= config->speed || config->speed == -1) {
                        uint32_t bytes;

                        fread(&bytes, sizeof(bytes), 1, stream.get());
                        content.append(QString(bytes));
                        fseek(stream.get(), sizeof(bytes), SEEK_CUR);

                        number_of_all_bytes += sizeof(bytes);
                        read_bytes += sizeof(bytes);
                    }
                    else {
                        while ((clock() - start) <= CLOCKS_PER_SEC);// qDebug() << "here is a loop";
                        //number_of_all_bytes += read_bytes;
                        read_bytes = 0;
                        start = clock();
                    }
                }
                file_read = true;
            }
            else
                error.append("[error] File " + config->path + name + " does not exist\n");
        }
    }
    reading_is_finished = true;
}

void print_time() {
    QString dots = "";
    while (!reading_is_finished) {
        clock_t now = clock();
        end_time = now;

        system("cls");

        qDebug() << "Reading continues" << dots << endl;
        dots += ".";
        qDebug() << "Time has passed " << static_cast<double>((now - start_time) / CLOCKS_PER_SEC) << " seconds" << endl;
        qDebug() << "How many values have already been read " << content.size() << endl;
    }
}

clock_t start_time, end_time;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Config config("config.toml");

    //qDebug() << qApp->applicationDirPath();

    if (!config.error) {
        thread content_reading(read_file_content, &config);
        thread duration(print_time);

        start_time = clock();

        content_reading.join();
        duration.join();

//        for (QString str : content) {
//            qDebug() << str << endl;
//        }

        clock_t took_time = end_time - start_time;

        if (!error.isEmpty())
            qDebug() << endl << error << endl;
        if (file_read) {
            qDebug() << "Whole reading took " << static_cast<double>(took_time / CLOCKS_PER_SEC) << " seconds" << endl;
            qDebug() << "Speed of reading " << static_cast<double>((number_of_all_bytes * CLOCKS_PER_SEC) / static_cast<unsigned long>(took_time)) << " bytes per second" << endl;
        }
    }

    return a.exec();
}
