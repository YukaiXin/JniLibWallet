
#ifndef __PACK_TYPE_H__
#define __PACK_TYPE_H__


#define PACK_OK        0x00
#define PACK_ERROR    0x01
#define PACK_MAX_LENGTH (18*18*8)
#define PACK_TINY_LENGTH    20
#define PACK_HEAD_LENGTH    2
#define PACK_CONTENT_LENGTH    18
#define PACK_TRANSFER_SIZE    2048


typedef enum {
    HEAD = 0x00,
    DATA = 0x01,
    SACK = 0x02,
    RACK = 0x03
}Data;

typedef enum {
    ACK_HEAD,
    ACK_GET_MAP,
    ACK_WITH_MAP,
    ACK_TAIL,
}ACKTYPE;

typedef struct pack_struct{
    unsigned char istiny:1;
    unsigned char msgid:2;
    unsigned char msglen:5;
    unsigned char msgindex;
    unsigned char data[PACK_CONTENT_LENGTH];
}PackData;


struct excahngebuf{
    int len;
    int head;
    int tail;
    unsigned char buf[PACK_TRANSFER_SIZE];
};

typedef enum{
    PACK_VALID,
    PACK_FORMAT_ERROR,
    PACK_OTHER_ERROR,
    
}PACKSTATUS;








typedef bool (*sendtiny)(unsigned char*data, int len);
typedef bool (*recvCallback)(int pos,unsigned char *data, int len, char* mac);
bool isMsgHead(PackData *pack, int *waitlen);
bool isMsgData(PackData *pack, PackData* map);
bool isMsgAck(PackData *pack);
void dumpdata(unsigned char *data, int len);

#endif
