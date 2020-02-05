#include <QCoreApplication>
#include <QDebug>
#include <memory>
#include <cstdio>
#include "global_var.h"
#include "config.h"

//#pragma warning(disable : 4996)
using namespace std;

bool reading_end = false;
FileReader file_reader;
Timer timer;
QThread reading_thread;
QThread timer_thread;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QObject::connect(&reading_thread, &QThread::start, &file_reader, &FileReader::run);
    QObject::connect(&timer_thread, &QThread::start, &timer, &Timer::run);

    QObject::connect(&file_reader, &FileReader::finished, &reading_thread, &QThread::terminate);
    QObject::connect(&timer, &Timer::finished, &timer_thread, &QThread::terminate);

    file_reader.moveToThread(&reading_thread);
    timer.moveToThread(&timer_thread);

    Config config("config.toml");

    //qDebug() << qApp->applicationDirPath();
    file_reader.setConfig(&config);
    file_reader.setReading_end(&reading_end);

    clock_t start_time;

    timer.setFile_reader(&file_reader);
    timer.setStart_time(&start_time);

    if (!config.error) {
        reading_thread.start();
        timer_thread.start();

        start_time = clock();

        for (QString str : file_reader.content()) {
            qDebug() << str << endl;
        }

        clock_t took_time = timer.end_time() - start_time;

        if (!file_reader.error().isEmpty())
            qDebug() << endl << file_reader.error() << endl;
        if (file_reader.number_of_all_bytes() > 0) {
            qDebug() << "Whole reading took " << static_cast<double>(took_time / CLOCKS_PER_SEC) << " seconds" << endl;
            qDebug() << "Speed of reading " << static_cast<double>((file_reader.number_of_all_bytes() * CLOCKS_PER_SEC) / static_cast<unsigned long>(took_time)) << " bytes per second" << endl;
        }
    }

    return a.exec();
}
