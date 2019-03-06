package Format;



import android.util.Log;

import com.iyich.wallet.lib.Convert;


import java.nio.ByteBuffer;


import static Format.PackageType.ACK_TYPE_GET_MAP;
import static Format.PackageType.ACK_TYPE_HEAD;
import static Format.PackageType.ACK_TYPE_TAIL;
import static Format.PackageType.ACK_TYPE_WITH_MAP;
import static Format.PackageType.DATA;
import static Format.PackageType.HEAD;
import static Format.PackageType.PACK_CONTENT_LENGTH;
import static Format.PackageType.PACK_TRANSFER_SIZE;
import static Format.PackageType.RACK;
import static Format.PackageType.SACK;

/**
 * Created by kxyu on 2018/11/21
 */

public class FastMode extends BaseFastmode {

    private static String TAG = "FastMode";
    static boolean isGetAck = false;
    static int recvBlockLen = 0;
    public static boolean isSendFlag = false;
    static boolean isRecvFlag = false;
    static boolean isSendExitSuc = false;

    static ByteBuffer sesendBlockMapNew ;
//    static PackData recvBlockMap = new PackData();

    byte[] recvData18 =  new byte[18];

    private FastModeCallback fastModeCallback;
    static byte[] revBuff ;

    public FastMode(FastModeCallback fastModeCallback) {
        this.fastModeCallback = fastModeCallback;
    }



    @Override
    boolean setBlock(byte[] block, int blockLen) {


        return false;
    }

    @Override
    boolean sendBlock(PackData packData) {

        return false;
    }

    @Override
    public boolean onSendBlock(byte[] bytes, int sendLength) {

        if(isSendFlag) return false;
        isSendFlag = true;
        return sendPackage(bytes, (short) sendLength);

    }

    private  boolean sendPackage(byte[] bytes, int sendLength) {

        ByteBuffer writeData = ByteBuffer.allocate(PACK_TRANSFER_SIZE);
        writeData.put(bytes);

        boolean isSendSuccess = true;
        int tlen;
        int retry = 0;
        int count = 0;

        isGetAck = false;
        retry = 0;
        while (true){
            if (count++ % 2500 == 0) {

                if (retry++ > 3) {
                    Log.i(TAG, "  send head package failed !!!!  ");
                    isSendFlag = false;
                    return false;
                }
                if (retry > 1) {
                    Log.d(TAG, "  send head data in cycle  ");
                }

                byte[] sendHeadData = new byte[20];
                sendHeadData[0] = (byte) ((2 << 3 )& 0xFF);
                sendHeadData[1] = (byte) (HEAD & 0xFF);
                sendHeadData[2] = (byte) (sendLength & 0xFF);
                sendHeadData[3] = (byte) ((sendLength >> 8) & 0xFF);
                fastModeCallback.sendData(sendHeadData);
            } else {
                sleep(400);//400 us 1
            }

            if (isGetAck) {
                Log.d(TAG, "  send Head success !!! ");
                count = 0;

                isGetAck = false;
                break;
            }

        }
        sesendBlockMapNew = ByteBuffer.allocate(20);

        while (true){
            writeData.rewind();
            //Send Data package
             Log.i(TAG, "  =====================    send data pack    ================");

            for (int i = 0; i < sendLength ; i += PACK_CONTENT_LENGTH){
                int pos = i / PACK_CONTENT_LENGTH;

                if(i + PACK_CONTENT_LENGTH < sendLength){
                    tlen = PACK_CONTENT_LENGTH;
                }else {
                    tlen = (char) (sendLength - i);
                }

                if(((getUnsignedByte(sesendBlockMapNew.get(pos/8)) & (1 << (pos % 8)))& 0xFF) == 0x00){

                    byte[] dataPackage = new byte[20];
                    dataPackage[0] = (byte) (((DATA << 1 ) & 0xFF | (tlen << 3) & 0xFF));
                    dataPackage[1] = (byte)(pos & 0xFF);

                    writeData.position(i);
                    Log.i(TAG,"  数据位置 "+ pos +" MapNew  position  :  "+sesendBlockMapNew.position()+" ByteBuffer position  "+ writeData.position()  + " 发送数据 ：  "+ Convert.bytesToHexString(dataPackage));
                    writeData.get(dataPackage, 2, tlen);
                    fastModeCallback.sendData(dataPackage);
                    sleep(300); //2
                }else {
                    Log.i(TAG,"  位置 "+pos+" 发送正确  : "+getUnsignedByte(sesendBlockMapNew.get(pos/8)));
                }

            }

            Log.i(TAG,"  ======================    数据发送完，send ACK package  ========================== ");
            //Send Ack package
            isGetAck = false;
            count = 0;
            retry = 0;

            sleep(400); //3

            while (true){
                if(count ++ % 2500 == 0){
                    if (retry++ > 5){
                        isSendFlag = false;
                        return false;
                    }
                    ByteBuffer ackDataTMp = ByteBuffer.allocate(20);  //0401000000000000000000000000000000000000 变成 0001000000000000000000000000000000000000 ？？？
                    ackDataTMp.put((byte) ((SACK << 1) & 0xFF));
                    ackDataTMp.put((byte) (ACK_TYPE_GET_MAP & 0xFF));

                    fastModeCallback.sendData(ackDataTMp.array());
                    Log.i(TAG, " send ack    ACK_TYPE_GET_MAP    "+ Convert.bytesToHexString(ackDataTMp.array()));
                    ackDataTMp.clear();
                }else {
                    Log.i(TAG, " send ack    ACK_TYPE_GET_MAP   等待  ");
                    sleep(400);//400 us 4
                }

                if(isGetAck){
                    isSendSuccess = true;
                    for (int i = 0; i < sendLength ; i += PACK_CONTENT_LENGTH){
                        int pos = i/PACK_CONTENT_LENGTH;
                        if(((getUnsignedByte(sesendBlockMapNew.get(pos/8)) & (1 << (pos % 8)))& 0xFF) == 0x00){
                            isSendSuccess = false;
                            break;
                        }
                    }

                    if(isSendSuccess){

                        byte[] ackTailData = new byte[20];//a
                        ackTailData[0] = (SACK << 1)&0xFF ;
                        ackTailData[1] = ACK_TYPE_TAIL & 0xFF;

                        sleep(400); //5

                        fastModeCallback.sendData(ackTailData);
                        sleep(400); //6


                        byte[] ackTailData1 = new byte[20];//a
                        ackTailData1[0] = (SACK << 1)&0xFF ;
                        ackTailData1[1] = ACK_TYPE_TAIL & 0xFF;
                        fastModeCallback.sendData(ackTailData1);

                        Log.i(TAG, " send ACK_TYPE_TAIL ");
                        isSendFlag = false;
                        sesendBlockMapNew.clear();
                        sesendBlockMapNew = null;
                        return true;
                    }else {
                        Log.d(TAG, "  send package lost data ");
                        break;
                    }
                }
            }
        }

    }

    private boolean onRecieve(byte[] bytes, int len) {

        PackData packData = new PackData();
        packData.tinyAndMsgIdAndMsgLen = bytes[0];
        packData.msgIdex = bytes[1];

        byte pos = 0;

        byte msgId = (byte) ((bytes[0] & 0x06)  >> 1);  // 00000110

        switch (msgId){
            case HEAD:

                recvBlockLen = bytes[2] + ((bytes[3] << 8) & 0xFF);

                Log.d(TAG, " onRecieve  HEAD  接受数据长度  ： "+recvBlockLen);
                if(isSendFlag){
                    if(recvBlockLen > 0){
                        isSendFlag = false;
                        Log.d(TAG," mRecvive: Force to stop send ? ");
                        sleep(800); //7
                    }
                    return false;
                }
                if(recvBlockLen == 0){
                  return false;
                }

                byte[] ackHead = new byte[20];
                ackHead[0] = ((RACK << 1) & 0xFFF);
                ackHead[1] =  ACK_TYPE_HEAD & 0xFF;

                fastModeCallback.sendData(ackHead);
                break;
            case DATA:
                pos = packData.msgIdex;
                if(pos == 0){
                    revBuff = new byte[2048]; //< 2048
                }
//                Log.i(TAG, " recvice DATA msgIdex  :   "+pos+"     string :  "+ Convert.bytesToHexString(bytes));

                recvData18[pos/8] |= (1 << (pos % 8)) & 0xFF ;
//                Log.i("kxyu_test"," pos   "+pos + "   DATA  "+Convert.bytesToHexString(revBuff)+" 长度  ：  "+recvBlockLen);

                System.arraycopy(bytes, 2, revBuff, pos*18, 18);
                break;
            case SACK:

                if(recvBlockLen == 0){
                    return true;
                }

                if(packData.msgIdex == ACK_TYPE_GET_MAP){//

                    isGetAck = true;
                      ByteBuffer byteBuffer = ByteBuffer.allocate(20);
                      byteBuffer.put((byte) (((18<< 3)&0xFF)|((RACK << 1) & 0xFFF)));
                      byteBuffer.put((byte) (ACK_TYPE_WITH_MAP & 0xFF));
                      byteBuffer.put(recvData18);
                    sleep(800);// 8
                    fastModeCallback.sendData(byteBuffer.array());

//                    Log.i(TAG, " mRecvive:    send  SACK ACK_TYPE_WITH_MAP   "+ Convert.bytesToHexString(byteBuffer.array()));

                }else if(packData.msgIdex == ACK_TYPE_TAIL) {//
//                    Log.i(TAG, " mRecvive:      SACK ACK_TYPE_TAIL     "+ Convert.bytesToHexString(bytes));

                    if(revBuff != null){
//                        fastModeCallback.revice(revBuff, recvBlockLen);
//                        Log.i("kxyu_r", " mRecvive:      Total Data     "+ Convert.bytesToHexString(revBuff) +" recvBlockLen "+recvBlockLen);
                    }
                    recvBlockLen = 0;
                    for (int i = 0; i < recvData18.length ; i ++ ){
                        recvData18[i] = (byte) 0x00;
                    }
                    revBuff = null;
                    return true;
                }else {
                    Log.i(TAG,"  SACK unkown state  "+packData.data[1]);
                }
                break;
            case RACK:
                if(isSendFlag && !isGetAck){
                    if(packData.msgIdex == ACK_TYPE_HEAD){ //9600000000000000000000000000000000000000
                        Log.i(TAG," onRecieve  RACK and ACK_HEAD   HD 确认头部 ");
                        isGetAck = true;
                        return true;
                    }else if(packData.msgIdex == ACK_TYPE_WITH_MAP){//9602010000000000000000000000000000000000
                        if(sesendBlockMapNew == null){
                            sesendBlockMapNew = ByteBuffer.allocate(20);
                        }
                        sesendBlockMapNew.clear();
                        sesendBlockMapNew.put(bytes, 2, 18);
                        isGetAck = true;
                        return true;
                    }else {
                        Log.i(TAG,"  ======RACK ============== unkown state  " + packData.data[1]);
                    }
                }else {
                    Log.i(TAG,"=====RACK ==================  isSendFlag : "+isSendFlag+" isGetAck :       "+isGetAck);
                }
                break;

        }
        return true;
    }

    @Override
    public boolean  onMRecieve(byte[] bytes, int len) {

        if(len < 20 ){
            Log.d(TAG,"Revice data length is not 20 byte");
            return false;
        }
        return onRecieve(bytes, len);
    }

    @Override
    void sentCallbackFlag() {
        isSendExitSuc = true;
    }


    //微秒
    private void sleep(int us){
        try {
            if(us < 1000){
                Thread.sleep(0, us*1000);
            }else {
                Thread.sleep(us / 1000);
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

    }

    public static int getUnsignedByte (byte data){ //将data字节型数据转换为0~255 (0xFF 即BYTE)。
        return data&0x0FF ;
    }

}
