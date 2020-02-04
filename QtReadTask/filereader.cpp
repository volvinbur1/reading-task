#include "filereader.h"

FileReader::FileReader(QObject *parent) : QObject(parent), m_error("")
{

}

Config *FileReader::config() const
{
    return m_config;
}

QList<QString> FileReader::content() const
{
    return m_content;
}

bool *FileReader::reading_end() const
{
    return m_reading_end;
}

QString FileReader::error() const
{
    return m_error;
}

void FileReader::run()
{
    for (QString name : m_config->file_names) {
        m_content.append("__" + name + "__");

        {
            FILE* file;// = fopen((config->path + name).toLocal8Bit().data(), "rb");
            fopen_s(&file,(m_config->path + name).toLocal8Bit().data(), "rb");
            auto stream = unique_ptr<FILE, decltype(&fclose)>(file, &fclose);

            if (stream != nullptr) {
                fseek(stream.get(), 0, SEEK_END);
                long size = ftell(stream.get());
                rewind(stream.get());

                //qDebug() << size << endl;

                clock_t start = clock();
                uint16_t read_bytes = 0;

                while (ftell(stream.get()) < size) {
                    if (read_bytes <= m_config->speed || m_config->speed == -1) {
                        uint32_t bytes;

                        fread(&bytes, sizeof(bytes), 1, stream.get());
                        m_content.append(QString(bytes));
                        fseek(stream.get(), sizeof(bytes), SEEK_CUR);

                        m_number_of_all_bytes += sizeof(bytes);
                        read_bytes += sizeof(bytes);
                    }
                    else {
                        while ((clock() - start) <= CLOCKS_PER_SEC);// qDebug() << "here is a loop";
                        //number_of_all_bytes += read_bytes;
                        read_bytes = 0;
                        start = clock();
                    }
                }
            }
            else
                m_error.append("[error] File " + m_config->path + name + " does not exist\n");
        }
    }
    *m_reading_end = true;

    emit finished();
}

void FileReader::setConfig(Config *config)
{
    if (m_config == config)
        return;

    m_config = config;
    emit configChanged(m_config);
}

void FileReader::setReading_end(bool *reading_is_finished)
{
    if (m_reading_end == reading_is_finished)
        return;

    m_reading_end = reading_is_finished;
    emit reading_endChanged(m_reading_end);
}
