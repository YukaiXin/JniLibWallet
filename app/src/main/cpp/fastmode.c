//
//  fastmode.c
//  BluetoothDemo
//
//  Created by apple on 2018/8/9.
//  Copyright © 2018年 Spring sky. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "package_type.h"
#include "fastmode.h"


static bool getack = false;
static unsigned int recv_block_len = 0;
static bool send_flag = false;
static bool recv_flag = false;
static PackData recv_block_map;
static PackData send_block_map;
static bool send_exit_succ = false;
struct excahngebuf recv_block_buff;  // MAX buffer 2048


static bool mRecvive(unsigned char*data, unsigned char len, sendtiny sen, recvCallback cb);

//static void doFillAckPack(PackData *tx, unsigned char ack, unsigned char state)
//{
//    memset(tx, 0x00, sizeof(PackData));
//    tx->msgid = ack;
//    tx->msgindex = 0;
//    tx->msglen = 1;
//    tx->data[0] = state;
//}

static void fill_head_package(PackData *pack, uint16_t length)
{
    uint8_t i;
    memset(pack, 0x00, sizeof(PackData));
    pack->msgid = HEAD;
    pack->msglen = 2;
    pack->msgindex = 0;
    pack->data[0] = length  & 0xff;
    pack->data[1] = (length >> 8) &0xff;
//    pack->data[2] = [GWBluetoothConnection currentConnection]._index;
    
    for(i = 2; i< 18; i++){
        pack->data[i] = i;
    }
     
    
}

static void fill_ack_package(PackData *pack, Data cmd,uint8_t state, uint8_t len, uint8_t *data)
{
    memset(pack, 0x00, sizeof(PackData));
    pack->msgid = cmd;
    pack->msglen = len;
    pack->msgindex = state;
    if(len > 0){
        memcpy(pack->data, data, len);
    }
}


static void fill_data_package(PackData *pack, uint16_t pos, uint8_t len, uint8_t *data)
{
    pack->msgid = DATA;
    pack->msglen = len;
    pack->msgindex = pos;
    memcpy(pack->data, &data[pos], len);
}

static bool onSendTinyPackage(unsigned char *senddata, unsigned int sendlen,  sendtiny send)
{
    PackData tx;
    unsigned int count = 0;
    bool ret = false;
    
    memset(&tx, 0x00, sizeof(PackData));
    tx.istiny = 1;
    tx.msglen = sendlen;
    tx.msgid = DATA;
    
    memcpy(tx.data, senddata, sendlen);
    send((unsigned char*)&tx, sizeof(PackData));
    getack = false;
    do{// wait ACK;
        if(getack){
            send_flag = false;
            break;
        }
        usleep(100);
        count++;
        if(count%10000 == 0){break;}
    }while(1);
    
    if(count >30000){
        printf("onSendBlock  send tiny failed\r\n");
        ret = false;
    }else{
        printf("onSendBlock  send tiny succ\r\n");
        ret = true;
    }
    send_flag = false;
    return ret;
}


static bool onSendPackage(unsigned char *senddata, unsigned int sendlen,  sendtiny send)
{
    PackData tx;
    // static PackData send_block_map;
    PackData *prxmap = &send_block_map;
    bool send_succ  = true;
    unsigned short i;
    unsigned char tlen;
    unsigned char retry = 0;
    unsigned int count = 0;
    PackData *pack = &tx;
    
    // send head of big package
    memset(prxmap, 0, sizeof(PackData));
    fill_head_package(&tx, sendlen);
    printf("ACK_WITH_MAP: send Head\r");
//    printf("：：%s\n",pack->data);
    getack = false;
    retry = 0;
    while(1){
        if(count++ % 2500 == 0){
            if(retry ++  > 3){
                printf("onSendBlock  send head failed\r\n");
                send_flag = false;
                return false;
            }
            if(retry > 1){
                printf("onSendBlock: send Head in cycle\r\n");
            }
            send((unsigned char*)&tx, sizeof(PackData));
        }else{
            usleep(400);
        }
        if(getack){
            printf("onSendBlock: send Head succ\r\n");
            count = 0;
            getack = false;
            break;;
        }
    }
    
    do{
        pack->msgid = DATA;
        count = 0;
        printf("send data len:%d\n", sendlen);
        for(i = 0; i < sendlen; i += PACK_CONTENT_LENGTH){
            unsigned char pos = i/PACK_CONTENT_LENGTH;
            printf("send pack:%d\n", i);
            if(i + PACK_CONTENT_LENGTH <= sendlen){
                tlen = PACK_CONTENT_LENGTH;
            }else{
                tlen = sendlen - i;
                memset(tx.data, 0x00, 18);
            }
            if(((prxmap->data[pos/8]) & (1 << (pos % 8))) == 0x00){
                pack->msglen = tlen;
                pack->msgindex = i/PACK_CONTENT_LENGTH;
                memcpy(pack->data, &senddata[i], tlen);
                send((unsigned char*)pack,sizeof(PackData));
                usleep(2000);
            }else{
                printf("map[%d] = %02x\n", pos/8, prxmap->data[pos/8]);
            }
        }
        
        // send ack
        getack = false;
        count = 0;
        retry = 0;
        fill_ack_package(pack, SACK,ACK_GET_MAP,0,NULL);
        usleep(1000);
        while(1){
            if(count++%2500 == 0){
                if(retry++ >5){
                    send_flag = false;
                    return false;
                }
                send((unsigned char*)pack,sizeof(PackData));
            }else{
                usleep(400);
            }
            
            if(getack){
                send_succ = true;
                for(i = 0; i < sendlen; i += PACK_CONTENT_LENGTH){
                    unsigned char pos = i/PACK_CONTENT_LENGTH;
                    if(((prxmap->data[pos/8]) & (1 << (pos % 8))) == 0x00){
                        send_succ = false;
                        break;
                    }
                }
                
                if(send_succ){
                    fill_ack_package(pack, SACK,ACK_TAIL,0,NULL);
                    usleep(1000);
                    send((unsigned char*)pack,sizeof(PackData));
                    usleep(1000);
                    send((unsigned char*)pack,sizeof(PackData));
                    printf("onSendBlock  send package succ\r\n");
                    send_flag = false;
                    return true;
                }else{
                    printf("onSendBlock  send package lost data\r\n");
                    break;
                }
            }
        }
    }while(1);
    send_flag = false;
    return true;
    
}

void sent_callbak_flag(void)
{
    send_exit_succ = true;
}

bool onSendBlock(unsigned char *senddata, unsigned int sendlen,  sendtiny send)
{
    if(send_flag){
        return false;
    }
    send_flag = true;
    if(sendlen >18){
        return onSendPackage(senddata, sendlen, send);
    }else{
        return onSendPackage(senddata, sendlen, send);
        //    return onSendTinyPackage(senddata, sendlen, send);
    }
    
}

static bool mRecvive(unsigned char*data, unsigned char len, sendtiny sen, recvCallback cb)
{
    PackData  *pack = (PackData  *)data;
    PackData  errpack;
    unsigned int i;
    unsigned char *rx_buff = recv_block_buff.buf;
    int pos;
    printf("ble2:mRecvive:");
    for(i = 0; i < len ;i++)
        printf("%02x ", data[i]);
    printf("\n");
    
    if(pack->istiny ){
        if(send_flag){
            send_flag = false;
            usleep(5000);
            // return false;
        }
        //send ack
        errpack.msgid = RACK;
        errpack.msgindex = 0;
        errpack.msglen = 0;
        memset(errpack.data, 0x00, PACK_CONTENT_LENGTH);
        sen((unsigned char*)&errpack, sizeof(PackData));
        cb(0, pack->data, pack->msglen);
        return true;
    }
    
    printf("msgid:%d \n", pack->msgid);
    switch(pack->msgid){
        case HEAD:
            recv_block_len = pack->data[0] + (pack->data[1] << 8);
            if(send_flag ){
                if((recv_block_len > 0)){
                    send_flag = false;
                    printf("mRecvive: Force to stop send?\r\n");
                    usleep(5000);
                }
                return false;
            }
            
            if(recv_block_len == 0){
                return false;
            }
            
            memset(&recv_block_map, 0x00,sizeof(recv_block_map));
            recv_block_map.msgid = RACK;
            recv_block_map.msgindex = ACK_HEAD;
            printf("mRecvive: send Head echo\r\n");
            sen((unsigned char*)&recv_block_map, sizeof(PackData));
            break;
            
        case SACK:
        {
            if (recv_block_len == 0){
                return true;
            }
            if(pack->msgindex == ACK_GET_MAP){
                getack = true;
                recv_block_map.msgid = RACK;
                recv_block_map.msgindex = ACK_WITH_MAP;
                usleep(5000);
                sen((unsigned char*)&recv_block_map, sizeof(PackData));
                
            }else if(pack->msgindex == ACK_TAIL){
                cb(0, rx_buff, recv_block_len);
                recv_block_len = 0x00;
                memset(&recv_block_map, 0x00, sizeof(recv_block_map));
                //send_flag = false;
                return true;
            }else{
                printf("SACK unkown state:%d", pack->msgindex);
            }
        }
            break;
        case RACK:
            printf("Rack \n");
            if (send_flag && !getack) {
                if(pack->msgindex == ACK_HEAD){
                    printf("RACK and ACK_HEAD\n");
                    getack = true;
                    return true;
                }else if(pack->msgindex == ACK_WITH_MAP){
                    memcpy(send_block_map.data, pack->data, PACK_CONTENT_LENGTH);
                    printf("RACK and ACK_WITH_MAP\n");
                    getack = true;
                    return true;
                }else{
                    printf("RACK unkown state:%d", pack->msgindex);
                }
            }else {
                printf("send_flag:%d getack:%d\n", send_flag, getack);
            }
            break;
        case DATA:
            pos = pack->msgindex;
            if((pack->msgindex +1) *18 < recv_block_len){
                recv_block_map.data[pack->msgindex/8] |= (1 << (pack->msgindex%8));
                memcpy(&rx_buff[18*pack->msgindex], pack->data, pack->msglen);
            }else{
                recv_block_map.data[pack->msgindex/8] |= (1 << (pack->msgindex%8));
                memcpy(&rx_buff[18*pack->msgindex], pack->data, pack->msglen);
            }
            break;
        default:
            break;
    }
    return true;
}




bool onMRecieve(unsigned char*data, unsigned char len, sendtiny sen, recvCallback cb)
{
    if(len == 0){
        printf("Length is not 20\r\n");
        return false;
    }
    return mRecvive(data, len, sen, cb);
}
