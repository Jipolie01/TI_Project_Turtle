#ifndef RF_MODULE_HPP
#define RF_MODULE_HPP

#include "communication.hpp"


class send : public communication
{
public:
    send(hwlib::target::pin_in_out & current_pin, int sending_frequency);
    
    void send_startbit() override;
    void send_byte(int byte) override;
    
};

class receive : public communication
{
public:
    receive(hwlib::target::pin_in_out & current_pin, int sending_frequency); 
    
    void get_startbit() override;
    int get_byte() override;

};

#endif // RF_MODULE_HPP
