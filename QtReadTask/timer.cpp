#include "timer.h"
#include <QDebug>

Timer::Timer(QObject *parent) : QObject(parent)
{

}

clock_t Timer::end_time() const
{
    return m_end_time;
}

clock_t *Timer::start_time() const
{
    return m_start_time;
}

FileReader *Timer::file_reader() const
{
    return m_file_reader;
}

void Timer::run()
{
    QString dots = "";
    while (!m_file_reader->reading_end()) {
        //clock_t now = clock();
        m_end_time = clock();

        system("cls");

        qDebug() << "Reading continues" << dots << endl;
        dots += ".";
        qDebug() << "Time has passed " << static_cast<double>((m_end_time - *m_start_time) / CLOCKS_PER_SEC) << " seconds" << endl;
        qDebug() << "How many values have already been read " << m_file_reader->content().size() << endl;
    }
    emit finished();
}

void Timer::setStart_time(clock_t *start_time)
{
    if (m_start_time == start_time)
        return;

    m_start_time = start_time;
    emit start_timeChanged(m_start_time);
}

void Timer::setFile_reader(FileReader *file_reader)
{
    if (m_file_reader == file_reader)
        return;

    m_file_reader = file_reader;
    emit file_readerChanged(m_file_reader);
}
