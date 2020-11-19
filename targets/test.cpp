#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <future>
#include <chrono>
#include "Scheduler.h"
#include "poster.h"
#include "listener.h"

void runScheduler() {
    subpub::Scheduler sched{};
    sched.start();
}

bool runPoster(std::string message) {
    subpub::poster publisher{};
    return (publisher.post(message) == 0);
}

bool runListener(std::string message) {
    subpub::listener subscriber{};
    return (subscriber.wait_for(message) == 0);
}

TEST_CASE("Subpub sniff test", "[scheduler]") {
    using namespace std::chrono_literals;

    std::string message{"Yo!"};
    auto futureScheduler = std::async(std::launch::async, runScheduler);
    auto futureListener  = std::async(std::launch::async, runListener, message);
    auto futurePoster    = std::async(std::launch::async, runPoster, message);

    auto statusListner = futureListener.wait_for(100ms);
    auto statusPoster  = futurePoster.wait_for(100ms);

    REQUIRE(statusListner == std::future_status::ready);
    REQUIRE(statusPoster  == std::future_status::ready);

    REQUIRE(futureListener.get());
    REQUIRE(futurePoster.get());
}