#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <ctime>
#include "filereader.h"

class Timer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(clock_t* start_time READ start_time WRITE setStart_time NOTIFY start_timeChanged)
    Q_PROPERTY(FileReader* file_reader READ file_reader WRITE setFile_reader NOTIFY file_readerChanged)

    clock_t* m_start_time;
    clock_t m_end_time;
    FileReader* m_file_reader;

public:
    explicit Timer(QObject *parent = nullptr);
    clock_t end_time() const;
    clock_t* start_time() const;
    FileReader* file_reader() const;

signals:
    void finished();
    void start_timeChanged(clock_t* start_time);
    void file_readerChanged(FileReader* file_reader);

public slots:
    void run();
    void setStart_time(clock_t* start_time);
    void setFile_reader(FileReader* file_reader);
};

#endif // TIMER_H
