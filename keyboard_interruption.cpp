#include "keyboard_interruption.hpp"

KeyboardInterruption::KeyboardInterruption(const char* msg)
: message(msg)
{
}

const char* KeyboardInterruption::what() const noexcept
{
    return message.c_str();
}

void keyboard_interruption_signal_handler(int signal)
{
    throw KeyboardInterruption("Ctrl-c captured");
}

void keyboard_interruption_register()
{
    struct sigaction interruption_handler;
    interruption_handler.sa_handler = keyboard_interruption_signal_handler;
    interruption_handler.sa_flags   = 0;
    sigemptyset(&interruption_handler.sa_mask);
    sigaction(SIGINT, &interruption_handler, nullptr);
}