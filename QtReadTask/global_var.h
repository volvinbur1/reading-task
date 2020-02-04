#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H
#include <QtGlobal>
#include <ctime>
#include <QString>
#include <QList>
#include <QThread>
#include "filereader.h"
#include "timer.h"

extern bool reading_end;
extern FileReader file_reader;
extern Timer timer;
extern QThread reading_thread;
extern QThread timer_thread;

#endif // GLOBAL_VAR_H
