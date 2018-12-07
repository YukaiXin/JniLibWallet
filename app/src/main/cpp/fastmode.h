//
//  fastmode.h
//  BluetoothDemo
//
//  Created by apple on 2018/8/9.
//  Copyright © 2018年 Spring sky. All rights reserved.
//

#ifndef __FASTMODE_H
#define __FASTMODE_H

#include "package_type.h"

bool setBlock(unsigned char *block, unsigned int blocklen);
bool sendBlock(PackData *data, sendtiny send);
bool onSendBlock(unsigned char *senddata, unsigned int sendlen, sendtiny send);
bool onMRecieve(unsigned char*data, unsigned char len, sendtiny send, recvCallback cb);
void sent_callbak_flag(void);
#endif /* fastmode_h */
