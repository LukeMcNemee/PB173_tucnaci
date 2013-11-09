#include "sharedlist.h"
#include "config.h"
namespace PenguinServer
{



SharedList::SharedList(): QObject(), mutex()
{
}

bool SharedList::addClient(ConnectedClient * cli)
{
    mutex.lock();
    QString q = cli->getName();
    if(clients.contains(q))
    {
        mutex.unlock();
        return false;
    }
    clients[q] = cli;
    mutex.unlock();
    return true;
}

bool SharedList::removeClient(const QString & tR)
{
    mutex.lock();
    int k = clients.remove(tR);
    mutex.unlock();
    if(k==0) return false;
    return true;


}

void SharedList::callAllClients()
{
    mutex.lock();
    QList<QString> toSent = clients.keys();
    QMap<QString, ConnectedClient*>::Iterator it;
    for(it = clients.begin(); it!= clients.end(); it++)
    {
        ConnectedClient * c = *it;
        emit c->sendUpdatedList(toSent);
    }

    mutex.unlock();


}

void SharedList::callClient(const QString &destName, const QString &srcName)
{
    if(!clients.contains(destName)) return;
    ConnectedClient* c = clients[destName];
    c->callRequest(REQUEST_CALL_TO_CLIENT_FROM_SERVER, clients[srcName]);


}

}//namespace PenguinServer
