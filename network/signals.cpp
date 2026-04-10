// signals.cpp
#include <csignal>
#include <cstdlib> 
#include "signals.hpp"

namespace {
    void on_exit(int) {
        // cleanup...
        std::exit(0);
    }
}

void setup_signal_handlers() {
    std::signal(SIGINT, on_exit);
}