#include "common.h"
#include "Scheduler.h"
#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

namespace {
    subpub::common::msg_t signalMsg;
    subpub::common::msg_t listenerMsg;
    subpub::common::msg_t notificationMsg;

    std::string delimiter = "***";

    void sendNotification(int msgQId);
}

namespace subpub {

Scheduler::Scheduler(void)
{
    key_t signalKey, listenerKey;
    int flags = IPC_CREAT;

    if ((signalKey   = ftok("keys/signal", 1)) == -1) {
        std::cout << "failed to get signal key" << std::endl;
    }
    if ((listenerKey = ftok("keys/listeners", 1)) == -1) {
        std::cout << "failed to get listener key" << std::endl;
    }

    if ((signalMsgQ = msgget(signalKey, flags)) == -1) {
        std::cout << "failed to get signal msgQ" << std::endl;
    }

    if ((listenerMsgQ = msgget(listenerKey, flags)) == -1) {
        std::cout << "failed to get listener msgQ" << std::endl;
    }
}

Scheduler::~Scheduler(void)
{
    signalProcessor.join();
    listenerProcessor.join();
}   

void Scheduler::start(void)
{
    signalProcessor   = std::thread(&Scheduler::listenForSignals, this);
    listenerProcessor = std::thread(&Scheduler::listenForListeners, this);
}

void Scheduler::listenForSignals(void)
{
    while(1) {
        if(msgrcv(signalMsgQ, &signalMsg, common::msgSize, 0, 0) < 0) {
            std::cout << "error receiving signal message" << std::endl;
            common::printMsgQErr();
        } else {
            std::cout << "received signal message: " <<  signalMsg.mtext << std::endl;
            notifyListeners(signalMsg.mtext);
        }
    }
}

void Scheduler::listenForListeners(void)
{
    while (1) {
        if(msgrcv(listenerMsgQ, &listenerMsg, common::msgSize, 0, 0) < 0) {
            std::cout << "error receiving listner message" << std::endl;
            common::printMsgQErr();
        } else {
            std::cout << "received listner message:" << listenerMsg.mtext << std::endl;
            std::string message{listenerMsg.mtext};
            std::string signal = message.substr(0, message.find(delimiter));
            std::string id = message.substr(message.find(delimiter)+3, message.length());
            std::cout << "signal: " << signal << std::endl;
            std::cout << "id: " << id << std::endl;
            addListenerForSignal(std::stoi(id), signal);
        }
    }
}

void Scheduler::addListenerForSignal(ProcessId listener, Signal signal)
{
    std::lock_guard<std::mutex> lock(mapProtecter);
    signalToListeners[signal].push(listener);
}

void Scheduler::notifyListeners(Signal signal)
{
    std::lock_guard<std::mutex> lock(mapProtecter);
    ProcessId currentId{};
    ListenerQueue& listeners{signalToListeners[signal]};
    
    while(!listeners.empty()) {
        currentId = listeners.front();
        listeners.pop();
        sendNotification(currentId);
    }
}

} // namespace subpub

namespace {
    void sendNotification(int msgQId)
    {
        if(msgsnd(msgQId, &notificationMsg, subpub::common::msgSize, 0) < 0) {
            std::cout << "error sending listener message" << std::endl;
            subpub::common::printMsgQErr();
        } else {
            std::cout << "sent listener message!" << std::endl;
            // return 0;
        }
    }
}