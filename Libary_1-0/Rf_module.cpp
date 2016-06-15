#include "Rf_module.hpp"

send::send(hwlib::target::pin_in_out & current_pin, int sending_frequency):
    communication(current_pin,sending_frequency)
{}

void send::send_startbit()
{
    send_bit(4 * sending_frequency);
}

void send::send_byte(int byte)
{
    send_startbit();
    for(int i = 7; i >= 0; i--)
    {
        auto bit = ((byte & (1 <<i)) != 0);
        send_bit(bit);
    }
}

receive::receive(hwlib::target::pin_in_out &current_pin, int sending_frequency):
    communication(current_pin, sending_frequency)
{}

void receive::get_startbit()
{
    auto data = current_pin.get();
    while(1)
    {
        if(!data)
        {
            data = current_pin.get();
        }
        else if(data)
        {
            auto find_startbit = get_bit();
            while(1)
            {
                if(find_startbit >= (3*sending_frequency))
                {
                    break;
                }
                else
                {
                    find_startbit = get_bit();
                }
            }
            break;
        }
    }
}

int receive::get_byte()
{
    get_startbit();
    int counter = 0;
    int byte = 0;
    while(counter < 8)
    {
        int time = get_bit();
        if(time == (2*sending_frequency))
        {
            byte = (byte | 1);
            if(counter != 7)
            {
                byte = byte << 1;
            }
            counter++;
        }
        else
        {
            byte = (byte | 0);
            if(counter != 7)
            {
                byte = byte << 1;
            }
            counter++;
        }
    }
    return byte;
}