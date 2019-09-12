#ifndef KEYBOARD_INTERRUPT_
#define KEYBOARD_INTERRUPT_

#include <string>
#include <signal.h>
#include <exception>

class KeyboardInterruption : public std::exception
{
    public: 
        KeyboardInterruption(const char*);
        const char* what() const noexcept override;

    private:
        std::string message;
};

void keyboard_interruption_signal_handler(int signal);
void keyboard_interruption_register();

#endif