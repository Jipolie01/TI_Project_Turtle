#include "communication.hpp"
#include "hwlib.hpp"

communication::communication(hwlib::target::pin_in_out & current_pin, int sending_frequency):
current_pin ( current_pin ),
sending_frequency ( sending_frequency )
{}

void communication::send_bit(int bit)
{
    current_pin.set(0);
    hwlib::wait_ms(4*sending_frequency);
    current_pin.set(1);
    hwlib::wait_ms(sending_frequency * (bit+1));
    current_pin.set(0);
}


int communication::get_bit()
{
    while(1)
    {
        if(!current_pin.get())
        {
            auto begin_time = hwlib::target::now_us();
            while(1)
            {
                if(current_pin.get())
                {
                    auto end_time = hwlib::target::now_us();
                    return (time - begin_time) / 1000;
                }
            }
        }
    }
}
