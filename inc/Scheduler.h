#ifndef SUBPUB_SCHEDULER_
#define SUBPUB_SCHEDULER_

#include <sys/ipc.h>
#include <thread>
#include <mutex>
#include <queue>
#include <map>

namespace subpub {

using Signal        = std::string;
using ProcessId     = int;
using MessageQId    = key_t;
using SubscriberQueue = std::queue<ProcessId>;

class scheduler
{
public:
    /**
     * Scheduler is responsible for listening to signals from posters,
     * and upon receiving a signal, notifying pending subscribers. It is also 
     * responsible for managing pending subscribers.
     *
     * Open message queues for both signals and listners.
     */
    scheduler();

    ~scheduler();

    /**
     * Begin listning for signals and subscribers.
     */
    void start(void);

    /**
     * Stop listning for signals and subscribers.
     */
    void stop(void);

private:
    /**
     * Begin receiving signals from posters.
     * Upon receiving a signal from a poster,
     * wake up all pending subscribers.
     */
    void listenForSignals(void);

    /**
     * Begin listening for subscribers.
     * Add subscriber to list of subscribers for requested signal.
     */
    void listenForSubscribers(void);

    /**
     * Add subscriber to list of pending subscribers
     */
    void addSubscriberForSignal(ProcessId subscriber, Signal signal);

    void notifySubscribers(Signal signal);

    std::map<Signal, SubscriberQueue> signalToSubscribers;
    std::mutex mapProtecter;

    MessageQId signalMsgQ;
    MessageQId subscriberMsgQ;

    std::thread signalProcessor;
    std::thread subscriberProcessor;
    bool continueProcessing;
};
} // namespace subpub

#endif // SUBPUB_SCHEDULER_