#include "Packet.h"

/** Create packet with specified data
 *  @param data Data to create packet with
 *  @param length Length of data
 */
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

/** Create packet with specified header
 *  @param header Pointer to header data
 */
Packet::Packet(Header* header) {

	//Allocate packet
	packet = (unsigned char*)malloc(sizeof(Header));
	memcpy(packet, header, sizeof(Header));
	packetLength = sizeof(Header);
}

/** Unload packet object data
 */
Packet::~Packet() {
	if (packet != NULL)
		free(packet);
}

/** Add data to packet
 *  @param data Data to add
 *  @param length Length of data to add
 */
void Packet::AddData(unsigned char* data, unsigned long length) {
	packet = (unsigned char*)realloc(packet, (packetLength+length+1));
	memcpy(packet+packetLength, data, length);
	packetLength += length;
	packet[packetLength] = 0x00;
}

/** Return raw packet data
 */
unsigned char* Packet::raw() {
	return packet;
}

/** Return raw packet data length
 */
unsigned long Packet::rawLength() {
	return packetLength;
}

/** Does the packet header contain a valid magic
 */
bool Packet::isValidMagic() {
	if (((Header*)packet)->magic == PACKET_MAGIC || ((Header*)packet)->magic == PACKET_MAGIC_CRYPT)
		return true;
	return false;
}

/** Does the packet header contain a valid checksum of the data
 */
bool Packet::isValidChecksum() {
	if (((Header*)packet)->checksum == calculateChecksum(data(), dataLength()))
		return true;
	return false;
}

/** Does the packet header suggest the packet is encrypted
 */
bool Packet::isEncrypted() {
	if (((Header*)packet)->magic == PACKET_MAGIC_CRYPT)
		return true;
	return false;
}

/** Return packet data
 */
unsigned char* Packet::data() {
	return packet+sizeof(Header);
}

/** Return packet data length
*/
unsigned long Packet::dataLength() {
	return packetLength-sizeof(Header);
}

/** Return packet magic
 */
unsigned long Packet::magic() {
	return (*(Header*)packet).magic;
}

/** Return packet checksum
 */
unsigned long Packet::checksum() {
	return (*(Header*)packet).checksum;
}

/** Run cipher on packet data (encrypt/decrypt)
 */
void Packet::crypt() {

	//Adjust packet header
	if ((*(Header*)packet).magic == PACKET_MAGIC) {
		(*(Header*)packet).magic = PACKET_MAGIC_CRYPT;
	} else {
		(*(Header*)packet).magic = PACKET_MAGIC;
	}

	for(unsigned long i=0;i<dataLength();i++)
		data()[i] ^= 0xC3;
}

/** Calculate checksum on given data
 *  @param data Data to run checksum on
 *  @param length Length of data to run checksum on
 */
unsigned long Packet::calculateChecksum(unsigned char* data, unsigned long length) {
	const int MOD_ADLER = 65521;
	unsigned long a = 1, b = 0;

    for (unsigned long i=0;i<length;i++){
        a = (a + data[i]) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }

    return (b << 16) | a;
}
