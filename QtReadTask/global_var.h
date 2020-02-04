#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H
#include <QtGlobal>
#include <ctime>
#include <QString>
#include <QList>

extern bool reading_is_finished;
extern quint64 number_of_all_bytes;
extern bool file_read;
extern clock_t start_time, end_time;
extern QString error;
extern QList<QString> content;

#endif // GLOBAL_VAR_H
