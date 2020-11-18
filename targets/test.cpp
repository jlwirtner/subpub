#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <thread>
#include <chrono>
#include "subpub.h"

void runScheduler() {
    subpub::Scheduler sched();
    sched.start();
}

bool runPoster(std::string message) {
    int check{};
    subpub::Poster poster();
    check = poster.post(message);
    return check == 0;
}

bool runListener(std::string message) {
    int check{};
    subpub::Listener listener();
    check = listener.wait_for(message);
    return check == 0;
}

TEST_CASE("Subpub sniff test", "[scheduler]") {
    using namespace std::chrono_literals;

    std::string message{"Yo!"};
    auto f_s = std::async(std::launch::async, runScheduler);
    auto f_l = std::async(std::launch::async, runListener, message);
    auto f_p = std::async(std::launch::async, runPoster, message);

    auto status_l = f_l.wait_for(100ms);
    auto status_p = f_p.wait_for(100ms);

    REQUIRE(status_l == std::future_status::ready);
    REQUIRE(status_p == std::future_status::ready);

    REQUIRE(f_l.get());
    REQUIRE(f_p.get());
}