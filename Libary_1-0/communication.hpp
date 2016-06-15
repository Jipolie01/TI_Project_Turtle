#ifndef COMMUNICATION_HPP
#define COMMUNICATION_HPP

class communication
{
protected:
    hwlib::target::pin_in_out & current_pin;
    int sending_frequency;
public:
    communication(hwlib::target::pin_in_out & current_pin, int sending_frequency);
    
    virtual void send_bit(int bit);
    virtual int get_bit();
    
    virtual void get_startbit() = 0;
    virtual void send_startbit() = 0;
    virtual int get_byte() = 0;
    virtual void send_byte(int byte) = 0;
    
};

#endif // COMMUNICATION_HPP
