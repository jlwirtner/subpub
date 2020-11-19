#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <future>
#include <chrono>
#include "scheduler.h"
#include "publisher.h"
#include "subscriber.h"

void runScheduler() {
    subpub::scheduler sched{};
    sched.start();
}

bool runPublisher(std::string message) {
    subpub::publisher publisher{};
    return (publisher.post(message) == 0);
}

bool runSubscriber(std::string message) {
    subpub::subscriber subscriber{};
    return (subscriber.wait_for(message) == 0);
}

TEST_CASE("Subpub sniff test", "[scheduler]") {
    using namespace std::chrono_literals;

    std::string message{"Yo!"};
    auto futureScheduler   = std::async(std::launch::async, runScheduler);
    auto futureSubscriber  = std::async(std::launch::async, runSubscriber, message);
    auto futurePublisher   = std::async(std::launch::async, runPublisher, message);

    auto statusSubscriber = futureSubscriber.wait_for(100ms);
    auto statusPublisher  = futurePublisher.wait_for(100ms);

    REQUIRE(statusSubscriber == std::future_status::ready);
    REQUIRE(statusPublisher  == std::future_status::ready);

    REQUIRE(futureSubscriber.get());
    REQUIRE(futurePublisher.get());
}