#ifndef CORE_H
#define CORE_H

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QDebug>


class Core
{
private:
    const QChar hexConst[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B','C','D','E','F'};
    const int bytesToReadFromFileHeader = 240000;

    QFile *dataFile;
    QVector<QString> names;
    QVector<QString> desc;
    QVector<QStringList> patern;
    QVector<QStringList> position;
    QVector<QStringList> string;

    bool initiated = false;

    // load file
    bool init();
public:
    Core(QFile *dataFile);

    // detect extention of the given file
    bool detect(QFile *file, QVector<QStringList> &output);

    // convert bytes to hex
    QString byte_2_str(char* bytes, int size);
};

#endif // CORE_H
