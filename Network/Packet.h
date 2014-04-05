#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PACKET_MAGIC 0x2E504B54 //.PKT
#define PACKET_MAGIC_CRYPT 0x2E504B43 //.PKC

class Packet {
public:

	struct Header {
		unsigned long magic;
		unsigned long checksum;
		unsigned long length;
	};

	Packet(unsigned char* data, unsigned long length);
	Packet(Header* header);
	~Packet();

	void AddData(unsigned char* data, unsigned long length);

	unsigned char* raw();
	unsigned long rawLength();

	unsigned long magic();
	unsigned long checksum();

	bool isValidMagic();
	bool isValidChecksum();
	bool isEncrypted();

	unsigned char* data();
	unsigned long dataLength();

	void crypt();

private:
	unsigned char* packet;
	unsigned long packetLength;

	unsigned long calculateChecksum(unsigned char* data, unsigned long length);
};

