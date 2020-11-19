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
using ListenerQueue = std::queue<ProcessId>;

class scheduler
{
public:
    /**
     * Scheduler is responsible for listening to signals from posters,
     * and upon receiving a signal, notifying pending listeners. It is also 
     * responsible for managing pending listeners.
     *
     * Open message queues for both signals and listners.
     */
    scheduler();

    ~scheduler();

    /**
     * Begin listning for signals and listeners.
     */
    void start(void);

private:
    /**
     * Begin receiving signals from posters.
     * Upon receiving a signal from a poster,
     * wake up all pending listeners.
     */
    void listenForSignals(void);

    /**
     * Begin listening for listeners.
     * Add listener to list of listeners for requested signal.
     */
    void listenForListeners(void);

    /**
     * Add listener to list of pending listeners
     */
    void addListenerForSignal(ProcessId listener, Signal signal);

    void notifyListeners(Signal signal);

    std::map<Signal, ListenerQueue> signalToListeners;
    std::mutex mapProtecter;

    MessageQId signalMsgQ;
    MessageQId listenerMsgQ;

    std::thread signalProcessor;
    std::thread listenerProcessor;
};
} // namespace subpub

#endif // SUBPUB_SCHEDULER_