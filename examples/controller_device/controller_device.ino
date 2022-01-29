// RPC as the controller device using Serial interface (ie. Software and Hardware serial)

#include <rpc.h>

RPC::RPCSERIAL interface(RPC::HARDWARE, RPC::SLAVE);


// Call Backs

// This example shows reading a Digital I/O pin remotely.
void digital_read_example()
{
    uint8_t state;
    if (interface.call_no_args(F("digital_read"), &state, sizeof(state)))
    {
        Serial.print(F("Remote Digital I/O State: "));
        Serial.println(state);
    }
}

// This example shows reading an Analog I/O pin remotely.
void analog_read_example()
{
    uint16_t state;
    if (interface.call_no_args(F("analog_read"), &state, sizeof(state)))
    {
        Serial.print(F("Remote Analog I/O State: "));
        Serial.println(state);
    }
}

// This example shows writing a Digital I/O pin remotely.
void digital_write_example()
{
    static uint8_t state = 0;
    if (interface.call(F("digital_write"), &state, sizeof(state)))
    {
        state = !state;
    }
}

// This example shows writing an Analog I/O pin remotely.
void analog_write_example()
{
    static uint8_t state = 0;
    if (interface.call(F("digital_write"), &state, sizeof(state)))
    {
        state = state + 1;
    }
}

// This example prints to the serial interface remotely.
void serial_print_example()
{
  const char str[] = "Hello World!";
  interface.call(F("serial_print"), (char*) str, strlen(str));
}

void setup()
{
    Serial.begin(115200);
    Serial.println(F("Testing RPC: Remote Control - As The Controller Device"));
    Serial1.begin(115200);
    interface.begin(&Serial1);
}

void loop()
{
    digital_read_example();
    analog_read_example();
    digital_write_example();
    analog_write_example();
    serial_print_example();
}
