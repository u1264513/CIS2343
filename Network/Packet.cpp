#include "Packet.h"

Packet::Packet(unsigned char* data, unsigned long length) {

	//Allocate packet
	packet = NULL;
	packet = (unsigned char*)realloc(packet, (sizeof(Header)+length));

	//Create header
	Header header;
	header.magic = PACKET_MAGIC;
	header.checksum = calculateChecksum(data, length);
	header.length = length;

	//Create packet
	memcpy(packet, &header, sizeof(Header));
	memcpy(packet+sizeof(Header), data, length);
	packetLength = sizeof(Header)+length;
}

Packet::Packet(Header* header) {

	//Allocate packet
	packet = (unsigned char*)malloc(sizeof(Header));
	memcpy(packet, header, sizeof(Header));
	packetLength = sizeof(Header);
}

Packet::~Packet() {
	if (packet != NULL)
		free(packet);
}

void Packet::AddData(unsigned char* data, unsigned long length) {
	packet = (unsigned char*)realloc(packet, (packetLength+length));
	memcpy(packet+packetLength, data, length);
	packetLength += length;
}

unsigned char* Packet::raw() {
	return packet;
}
unsigned long Packet::rawLength() {
	return packetLength;
}

bool Packet::isValidMagic() {
	if (((Header*)packet)->magic == PACKET_MAGIC || ((Header*)packet)->magic == PACKET_MAGIC_CRYPT)
		return true;
	return false;
}
bool Packet::isValidChecksum() {
	if (((Header*)packet)->checksum == calculateChecksum(data(), dataLength()))
		return true;
	return false;
}

bool Packet::isEncrypted() {
	if (((Header*)packet)->magic == PACKET_MAGIC_CRYPT)
		return true;
	return false;
}

unsigned char* Packet::data() {
	return packet+sizeof(Header);
}
unsigned long Packet::dataLength() {
	return packetLength-sizeof(Header);
}

unsigned long Packet::magic() {
	return (*(Header*)packet).magic;
}
unsigned long Packet::checksum() {
	return (*(Header*)packet).checksum;
}

void Packet::crypt() {

	//Adjust packet header
	if ((*(Header*)packet).magic == PACKET_MAGIC) {
		(*(Header*)packet).magic = PACKET_MAGIC_CRYPT;
	} else {
		(*(Header*)packet).magic = PACKET_MAGIC;
	}

	//Basic XOR cipher
	for(unsigned long i=0;i<dataLength();i++)
		data()[i] ^= 0xC3;
}

unsigned long Packet::calculateChecksum(unsigned char* data, unsigned long length) {
	const int MOD_ADLER = 65521;
	unsigned long a = 1, b = 0;

    for (unsigned long i=0;i<length;i++){
        a = (a + data[i]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }

    return (b << 16) | a;
}
