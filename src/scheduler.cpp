#include "common.h"
#include "scheduler.h"
#include <chrono>
#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

namespace {
    subpub::common::msg_t signalMsg {};
    subpub::common::msg_t listenerMsg {};
    subpub::common::msg_t notificationMsg {};

    std::string delimiter = "***";

    void sendNotification(int msgQId);
}

namespace subpub {

scheduler::scheduler(void) : continueProcessing{false}
{
    key_t signalKey, subscriberKey;
    int flags = IPC_CREAT;

    if ((signalKey   = ftok("keys/signal", 1)) == -1) {
        std::cout << "failed to get signal key" << std::endl;
    }
    if ((subscriberKey = ftok("keys/subscribers", 1)) == -1) {
        std::cout << "failed to get subscriber key" << std::endl;
    }

    if ((signalMsgQ = msgget(signalKey, flags)) == -1) {
        std::cout << "failed to get signal msgQ" << std::endl;
    }

    if ((subscriberMsgQ = msgget(subscriberKey, flags)) == -1) {
        std::cout << "failed to get subscriber msgQ" << std::endl;
    }
}

scheduler::~scheduler(void)
{
    continueProcessing = false;
    signalProcessor.join();
    subscriberProcessor.join();
}   

void scheduler::start(void)
{
    continueProcessing = true;
    signalProcessor   = std::thread(&scheduler::listenForSignals, this);
    subscriberProcessor = std::thread(&scheduler::listenForSubscribers, this);
}

void scheduler::stop(void)
{
    continueProcessing = false;
}

void scheduler::listenForSignals(void)
{
    using namespace std::chrono_literals;

    while(continueProcessing) {
        if(msgrcv(signalMsgQ, &signalMsg, common::msgSize, 0, IPC_NOWAIT) < 0) {
            // std::cout << "error receiving signal message" << std::endl;
            // common::printMsgQErr();
        } else {
            std::cout << "received signal message: " <<  signalMsg.mtext << std::endl;
            notifySubscribers(signalMsg.mtext);
        }
        std::this_thread::sleep_for(10ms);
    }
}

void scheduler::listenForSubscribers(void)
{
    using namespace std::chrono_literals;

    while (continueProcessing) {
        if(msgrcv(subscriberMsgQ, &listenerMsg, common::msgSize, 0, IPC_NOWAIT) < 0) {
            // std::cout << "error receiving listner message" << std::endl;
            // common::printMsgQErr();
        } else {
            std::cout << "received listner message:" << listenerMsg.mtext << std::endl;
            std::string message{listenerMsg.mtext};
            std::string signal = message.substr(0, message.find(delimiter));
            std::string id = message.substr(message.find(delimiter)+3, message.length());
            std::cout << "signal: " << signal << std::endl;
            std::cout << "id: " << id << std::endl;
            addSubscriberForSignal(std::stoi(id), signal);
        }
        std::this_thread::sleep_for(10ms);
    }
}

void scheduler::addSubscriberForSignal(ProcessId subscriber, Signal signal)
{
    std::lock_guard<std::mutex> lock(mapProtecter);
    signalToSubscribers[signal].push(subscriber);
}

void scheduler::notifySubscribers(Signal signal)
{
    std::lock_guard<std::mutex> lock(mapProtecter);
    ProcessId currentId{};
    SubscriberQueue& subscribers{signalToSubscribers[signal]};
    
    while(!subscribers.empty()) {
        currentId = subscribers.front();
        subscribers.pop();
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