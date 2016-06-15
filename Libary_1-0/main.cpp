#include "hwlib.hpp"
#include "Rf_module.hpp"

int main(void)
{
    auto pin = hwlib::target::pin_in_out(1,23);
    pin.direction_set_output();
    
	receive goo(pin, 10);
	return 0;
}
