/* tscheck.c */
/* Copyright Acorn Computers Limited 1997 */

#include <stdio.h>
#include <stdlib.h>

#define PACKETSIZE 188
#define SYNC 0x47

int bytesleft;
FILE *f = stdin;
int pmt_pid = -1;

unsigned int crcblock(unsigned char *block, int len)
{
    unsigned int word = 0xFFFFFFFF;
    unsigned int eorrot;
    int i, j, byte;
    for (j=0; j<len; j++)
    {
        byte = block[j];
        eorrot = 0x608EDB02;
        word = byte ^ ((word >> 24) | (word << 8));
        for (i = 7; i >= 0; i--) /* for each input bit */
        {
            if (word & (1<<i)) word ^= eorrot;
            eorrot = (eorrot >> 1) | (eorrot << 31);
        }
    }
    return word;
}

int bget(void)
{
    int c;
    if (bytesleft <= 0) {
        printf("Packet data longer than packet length\n");
        exit(1);
    }
    c = fgetc(f);
    if (c == EOF) {
        if (bytesleft != PACKETSIZE) {
            printf("Got EOF in middle of packet\n");
            exit(1);
        } else {
            printf("Finished\n");
            exit(0);
        }
    }
    bytesleft--;
    return c;
}

void skipbytes(int count) {
    while (count--) bget();
}

void skiptonextpacket(void) {
    skipbytes(bytesleft);
}

void skipadaptationfield(void) {
    skipbytes(bget()); /* first byte is adaptation field length, followed by that number of bytes */
}

int readsection(unsigned char *buf, int table_id) {
    unsigned int storedcrc, correctcrc;
    int slen; /* section length */
    int i;

    skipbytes(bget()); /* first byte is pointer field, followed by that number of bytes */
    buf[0] = bget();
    if (buf[0] == table_id) {
        buf[1] = bget();
        buf[2] = bget();
        slen = buf[2] | ((buf[1] & 0x0F) << 8);
        for (i=3; i<slen+3; i++) {
            buf[i] = bget();
        }
        storedcrc = (buf[slen+2] & 0xFF) | (buf[slen+1] << 8) | (buf[slen+0] << 16) | (buf[slen-1] << 24);
        correctcrc = crcblock(buf, slen-1);
        if (storedcrc == correctcrc) {
            printf("CRC OK\n");
        } else {
            printf("CRC incorrect, was 0x%08X but should be %08X\n", storedcrc, correctcrc);
        }
    } else {
        printf("table_id wrong: was 0x%02X not 0x%02X\n", buf[0], table_id);
    }
    return slen+3; /* include 1st 3 bytes in count */
}


void processPAT(void)
{
    unsigned char buf[PACKETSIZE];
    int i;
    int pid;
    int bytes;
    int program;
    int last_pmt_pid = pmt_pid;

    printf("PAT: ");
    bytes = readsection(buf, 0x00);
    if (bytes) {
        pmt_pid = -1;
	for (i = 8; i < bytes-4; i+=4) {
	    program = buf[i+1] | (buf[i] << 8);
	    pid = buf[i+3] | ((buf[i+2] & 0x1F) << 8);
	    if (program != 0)
	    {
	        pmt_pid = pid;
	        if (pmt_pid != last_pmt_pid) printf("PMT PID set to 0x%04X\n", pmt_pid);
	    }
	}
	if (pmt_pid == -1) printf("PMT PID unset\n");
    }
    skiptonextpacket();
}

void processPMT(void)
{
    unsigned char buf[PACKETSIZE];
    int bytes;

    printf("PMT: ");
    bytes = readsection(buf, 0x02);
    skiptonextpacket();
}

int main(void)
{
    int c, f1, f2, pid;
    int packetcount = 0;

    do {
        bytesleft = PACKETSIZE;
        c = bget();
        if (c != SYNC) {
            printf("Stream doesn't start on packet boundary\nSkipping to start of packet\n");
            while (bget() != SYNC) {}
            bytesleft = PACKETSIZE-1;
        }
        f1 = bget(); /* transport_error_indicator, payload_unit_start_indicator, transport_priority, PID[12:8] */
        pid = bget() | ((f1 & 0x1F) << 8);
        /* printf("PID = 0x%04X\n", pid); */
        f2 = bget(); /* transport_scrambling_control, adaptation_field_control, continuity_counter */
        if (f2 & 0x20) skipadaptationfield();
        if ((f2 & 0x10) && (f1 & 0x40)) {
            /* is payload, and payload_unit_start_indicator is true */
            if (pid == 0) {
                processPAT();
            } else if (pid == pmt_pid) {
                processPMT();
            } else {
                skiptonextpacket();
            }
        } else {
            /* No payload, so skip to next packet */
            skiptonextpacket();
        }
        packetcount++;
    } while (!feof(f));
    return 0;
}
