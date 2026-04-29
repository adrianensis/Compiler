#pragma once

#include <csignal>

class SignalUtils
{
public:
    static void registerSignalHandler(int sig)
    {
        std::signal(sig, signalHandler);
    }

    static void signal(int sig)
    {
        std::raise(sig);
    }

    static void breakpointTrap()
    {
        #if defined(SIGTRAP)
            signal(SIGTRAP);
        #else
            signal(SIGABRT);
        #endif
    }
private:
    // Signal handler function
    static void signalHandler(int sig) {
        // Optionally exit the program after handling
        exit(sig);
    }
};