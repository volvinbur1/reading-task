#ifndef FILEREADER_H
#define FILEREADER_H

#include <QObject>
#include <QString>
#include <QList>
#include "config.h"

class FileReader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Config* config READ config WRITE setConfig NOTIFY configChanged)
    Q_PROPERTY(bool* reading_end READ reading_end WRITE setReading_end NOTIFY reading_endChanged)

    Config* m_config;
    QList<QString> m_content;
    quint64 m_number_of_all_bytes = 0;
    bool* m_reading_end;
    QString m_error;

public:
    explicit FileReader(QObject *parent = nullptr);
    Config* config() const;
    QList<QString> content() const;
    quint64 number_of_all_bytes() const;
    bool* reading_end() const;
    QString error() const;

signals:
    void finished();
    void configChanged(Config* config);
    void reading_endChanged(bool* reading_is_finished);

public slots:
    void run();
    void setConfig(Config* config);
    void setReading_end(bool* reading_is_finished);
};

#endif // FILEREADER_H
