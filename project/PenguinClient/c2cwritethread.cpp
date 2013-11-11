#include "c2cwritethread.h"
#include "./../messageenvelop.h"

namespace PenguinClient
{
C2CWriteThread::C2CWriteThread(QObject *parent) :
    QThread(parent), quit(false)
{
}

C2CWriteThread::~C2CWriteThread(){

}

void C2CWriteThread::startOutput(const QHostAddress &hostName, const quint16 port){
    qDebug() << "Write init";
    QMutexLocker locker(&mutex);
    this->hostName = hostName;
    this->port = port;
    if (!isRunning()){
        start();
    }
}

void C2CWriteThread::run(){
    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(hostName, port);
    QDataStream stream(tcpSocket);
    MessageEnvelop e;
    QList<QString> list;
    for (int i = 0; i < 10; i++){
        qDebug() << "generating data";
        std::string str = random_string(1024);
        QString qstr = QString::fromStdString(str);
        list.append(qstr);
    }
    e.setClients(list);
    stream << e;
    qDebug() << "data sent";
}

int C2CWriteThread::encryptDatagram(char* in, char* out, int length){
    for(int i = 0; i < length; i++){
        out[i] = in[i];
    }
    return 0;
}

std::string C2CWriteThread::random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

}
