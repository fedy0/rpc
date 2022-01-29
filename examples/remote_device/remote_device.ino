
// RPC as the remote device using Serial interface (ie. Software and Hardware serial)

#include <rpc.h>

RPC::rpc_scratch_buffer<256> scratch_buffer; // All RPC objects share this buffer.
RPC::rpc_callback_buffer<5> callback_buffer;         // All RPC objects share this buffer.
RPC::RPCSERIAL interface(RPC::HARDWARE, RPC::SLAVE);

// Call Backs

size_t digital_read_example(void *out_data) {
  // Get what we want to return into a variable.
  uint8_t state = digitalRead(4);

  // Move that variable into a transmit buffer.
  memcpy(out_data, &state, sizeof(state));

  // Return how much we will send.
  return sizeof(state);
}

size_t analog_read_example(void *out_data) {
  // Get what we want to return into a variable.
  uint16_t state = analogRead(A0);

  // Move that variable into a transmit buffer.
  memcpy(out_data, &state, sizeof(state));

  // Return how much we will send.
  return sizeof(state);
}

void digital_write_example(void *in_data, size_t in_data_len) {
  // Create the primitive or complex data type on the stack.
  uint8_t state;

  // Check that we received the amount of data we expected.
  if (in_data_len != sizeof(state)) return;

  // Copy what we received into our data type container.
  memcpy(&state, in_data, sizeof(state));

  // Use it now.
  digitalWrite(2, state);
}

void analog_write_example(void *in_data, size_t in_data_len) {
  // Create the primitive or complex data type on the stack.
  uint8_t state;

  // Check that we received the amount of data we expected.
  if (in_data_len != sizeof(state)) return;

  // Copy what we received into our data type container.
  memcpy(&state, in_data, sizeof(state));

  // Use it now.
  analogWrite(0, state);
}

void serial_print_example(void *in_data, size_t in_data_len) {
  // Create the string on the stack (extra byte for the null terminator).
  char buff[in_data_len + 1]; memset(buff, 0, in_data_len + 1);

  // Copy what we received into our data type container.
  memcpy(buff, in_data, in_data_len);

  // Use it now.
  Serial.println(buff);
}

// NOTE: The string name can be anything below. It just needs to match between the master/slave devices.

void setup() {

  // For MCP2515 CAN we might need to change the default CAN settings for the Arduino Uno.
  //
  // CAN.setPins(9, 2); // CS & INT
  // CAN.setClockFrequency(16E6); // 16 MHz

  interface.register_callback(F("digital_read"), digital_read_example);
  interface.register_callback(F("analog_read"), analog_read_example);
  interface.register_callback(F("digital_write"), digital_write_example);
  interface.register_callback(F("analog_write"), analog_write_example);
  interface.register_callback(F("serial_print"), serial_print_example);

  // Startup the RPC interface and a debug channel.
  Serial.begin(115200);
  Serial.println(F("Testing RPC: As The Remote Device"));
#if defined (ESP32)
  Serial1.begin(115200, SERIAL_8N1, 21, 22);   // NOTE: SIMXX Series modem only support baud rates 0(auto-bauding) & 1200-115200 only
#else
  Serial1.begin(115200);                                          // NOTE: SIMXX Series modem only support baud rates 0(auto-bauding) & 1200-115200 only
#endif
  interface.begin(&Serial1);
}

// Once all call backs have been registered we can start
// processing remote events.

void loop() {
  interface.loop();
}
