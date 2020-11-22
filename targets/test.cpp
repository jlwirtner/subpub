#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <future>
#include <chrono>
#include <thread>
#include <condition_variable>
#include "scheduler.h"
#include "publisher.h"
#include "subscriber.h"

#include <iostream>

namespace {
    std::mutex m_sched;
    std::condition_variable cv_sched;
    bool done  = false;
}

void runScheduler() {
    subpub::scheduler sched{};
    sched.start();
    // wait until tests are done
    std::unique_lock<std::mutex> lk(m_sched);
    cv_sched.wait(lk, [] { return done; });
    sched.stop();
}

bool runPublisher(std::string message) {
    subpub::publisher publisher{};
    bool check{publisher.post(message) == 0};
    return check;
}

bool runSubscriber(std::string message) {
    subpub::subscriber subscriber{};
    bool check {subscriber.wait_for(message) == 0};
    // notify scheduler test is done
    done = true;
    cv_sched.notify_one();
    return check;
}

TEST_CASE("Subpub sniff test", "[scheduler]") {
    using namespace std::chrono_literals;

    std::string message{"Yo!"};
    auto futureScheduler   = std::async(std::launch::async, runScheduler);
    auto futureSubscriber  = std::async(std::launch::async, runSubscriber, message);
    std::this_thread::sleep_for(40ms);
    auto futurePublisher   = std::async(std::launch::async, runPublisher, message);

    auto statusSubscriber = futureSubscriber.wait_for(100ms);
    auto statusPublisher  = futurePublisher.wait_for(100ms);

    REQUIRE(statusSubscriber == std::future_status::ready);
    REQUIRE(statusPublisher  == std::future_status::ready);

    REQUIRE(futureSubscriber.get());
    REQUIRE(futurePublisher.get());

}