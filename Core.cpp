#include "Core.h"

Core::Core(QFile *dataFile)
{
    this->dataFile = dataFile;

    this->initiated = init();
}

bool Core::init()
{
    if(!dataFile->open(QIODevice::ReadOnly))
        return false;

    QTextStream input(dataFile);

    while(!input.atEnd())
    {
        QString line = input.readLine();

        QStringList list = line.split(";");

        QString name = list[0];
        QString des = list[1];

        int patternSize = list[2].toInt();

        QStringList patterns;
        QStringList pos;

        for(int i = 0 ; i < patternSize * 2;)
        {
            patterns = patterns << list[3 + i];
            i++;
            pos = pos << list[3 + i];
            i++;
        }

        int stringSize = list[3 + patternSize * 2].toInt();

        QStringList strings;

        for(int i = 0 ; i < stringSize; i++)
        {
            strings = strings << list[4 + patternSize * 2 +  i];
        }

        names.push_front(name);
        desc.push_front(des);
        patern.push_front(patterns);
        position.push_front(pos);
        string.push_front(strings);
    }

    return true;
}

QString Core::byte_2_str(char* bytes, int size) {
  QString str;
  for (int i = 0; i < size; ++i) {
    const char ch = bytes[i];
    str.append(hexConst[(ch  & 0xF0) >> 4]);
    str.append(hexConst[ch & 0xF]);
  }
  return str;
}

bool Core::detect(QFile *file, QVector<QStringList> &output)
{
    if(!initiated) return false;

    if(!file->open(QIODevice::ReadOnly))
        return false;

    long fileLength = file->bytesAvailable();
    bool readWholeFile = fileLength > bytesToReadFromFileHeader ? false:true;

    QDataStream data(file);
    char *header;
    int HLen;

    if(readWholeFile)
    {
        HLen = fileLength;
        header = new char[fileLength];
        data.readRawData(header, fileLength);
    }else{
        HLen = bytesToReadFromFileHeader;
        header = new char[bytesToReadFromFileHeader];

        // read (bytesToReadFromFileHeader / 2) bytes from start of file
        data.readRawData(header, bytesToReadFromFileHeader / 2);

        // read (bytesToReadFromFileHeader / 2) bytes from end of file
        long bytesToSkip = fileLength - bytesToReadFromFileHeader;
        data.skipRawData(bytesToSkip);
        data.readRawData((header + bytesToReadFromFileHeader / 2) , bytesToReadFromFileHeader / 2);
    }

    // convert readable bytes to character
    QString headerSTR;
    for(int b = 0; b < HLen; b++){
        unsigned char c = header[b];
        if(c >= 32 && c <= 126){
            headerSTR.append(QChar(c));
        }else{
            headerSTR.append(".");
        }
    }
    headerSTR = headerSTR.toUpper();

    // convert all bytes to HEX
    QString headerHex = byte_2_str(header, HLen);

    // the detection process begins ...

    int sumOfPoints = 0;

    int *points = new int[names.size()];
    for(int i = 0; i < names.size(); i++)
    {
        int point = 0;

        QStringList patterns = patern[i];
        QStringList pos = position[i];

        int numberOfMatched = 0;
        int len = 0;
        for(int k = 0; k < patterns.size(); k++)
        {
            QString st = patterns[k];
            int p = pos[k].toInt();

            QString mid = headerHex.mid(p, st.length());
            if(mid.compare(st) == 0){
                numberOfMatched ++;
                len += st.length();
            }else{
                break;
            }
        }

        if( numberOfMatched < patterns.size() )
        {
            points[i] = 0;
            continue;
        }
        point += 200 * (len / (double) patterns.size());// * len;
        QStringList strings = string[i];

        numberOfMatched = 0;
        for(int k = 0; k < strings.size(); k++)
        {
            QString st = strings[k];

            int index = headerSTR.indexOf(st, 0);

            if(index != -1){
                numberOfMatched ++;
                if(!names[i].isEmpty() && st.indexOf(names[i]) != -1)
                {
                    numberOfMatched ++;
                }
                if(!desc[i].isEmpty() && desc[i].indexOf(st,0,Qt::CaseInsensitive) != -1){

                    numberOfMatched ++;
                }
            }
        }
        if(numberOfMatched < strings.size())
        {
            points[i] = 0;
            continue;
        }

        points[i] = point + numberOfMatched * 100000;

        sumOfPoints += points[i];

    }

    int *sortedPoints = new int[names.size()];

    for(int i = 0; i < names.size(); i++)
    {
        sortedPoints[i] = i;
    }

    for(int i = 0; i < names.size(); i++)
    {
        for(int k = 0; k < names.size() - 1; k++)
        {
            if(points[sortedPoints[k]] < points[sortedPoints[k + 1]] )
            {
                int t = sortedPoints[k];
                sortedPoints[k] = sortedPoints[k + 1];
                sortedPoints[k + 1] = t;
            }
        }
    }

    for(int i = 0; i < names.size(); i++)
    {
        if(points[sortedPoints[i]] == 0)
            break;

        QStringList list;
        list.append(names[sortedPoints[i]]);
        list.append(desc[sortedPoints[i]]);
        list.append(QString::number(points[sortedPoints[i]]));
        list.append("%" + QString::number(points[sortedPoints[i]] / static_cast<double>(sumOfPoints) * 100.0));

        output.push_back(list);
    }

    return true;
}
