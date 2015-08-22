// #include <XBee.h>

// setup
XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
Rx16Response rx = Rx16Response();
// uint8_t rx_data = 1;

// wait counter
// int delay_counter = 0;

// command variable defined by rx packet
int command = 1;

// payload
uint8_t packet[8];
uint8_t packet_copy[8];
// uint8_t packet[8] = {6, 100, 2, 0, 0, 0, 0, 0};

// unsigned long start = millis();
// uint8_t payload[8];

// TxStatusResponse txStatus = TxStatusResponse();
// Tx16Request tx2 = Tx16Request(0x1020, payload, sizeof(payload));
// Tx16Request tx3 = Tx16Request(0x1030, payload, sizeof(payload));
// Tx16Request tx4 = Tx16Request(0x1040, payload, sizeof(payload));
// Tx16Request tx5 = Tx16Request(0x1072, payload, sizeof(payload));
// Tx16Request tx6 = Tx16Request(0x1060, payload, sizeof(payload));

// Tx16Request address_list[] = {tx2, tx3, tx4, tx5, tx6};

void receive_command(){
	xbee.readPacket();

	if(xbee.getResponse().isAvailable()){

		Serial.println("XBee Available!");

		if(xbee.getResponse().getApiId() == RX_16_RESPONSE){

			Serial.println("Packet Received!");

			xbee.getResponse().getRx16Response(rx);
			command = rx.getData(0);
			Serial.println(command);
		}

	}else if(xbee.getResponse().isError()){
		
		Serial.print("XBee Error code: ");
		Serial.println(xbee.getResponse().getErrorCode());

	// }else{
	// 	Serial.println("Unknown Error!");
	}
}


void receive_packet(){

	xbee.readPacket();

	if(xbee.getResponse().isAvailable()){

		Serial.println("XBee Available!");

		if(xbee.getResponse().getApiId() == RX_16_RESPONSE){

			Serial.println("Packet Received!");

			xbee.getResponse().getRx16Response(rx);

			command = rx.getData(0);
			
			for(int i = 0; i < 8; i++){
				packet[i] = rx.getData(i);
				Serial.print(packet[i]);
				Serial.print(", ");
			}

			Serial.println();
		}

	}else if(xbee.getResponse().isError()){
		
		Serial.print("XBee Error code: ");
		Serial.println(xbee.getResponse().getErrorCode());

	// }else{
	// 	Serial.println("Unknown Error!");
	}
}

bool check_for_new_data(){
	for(int i = 0; i < 8; i++){
		if(packet[i] != packet_copy[i]){
			Serial.println("New Packet Data.");
			memcpy(packet_copy, packet, 8);
			return true;
		}
	}
	return false;
}