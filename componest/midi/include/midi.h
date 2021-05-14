
#ifndef _MIDI_H_
#define _MIDI_H_

/*
    整个文件组成： <文件头块> + <音轨块数据>
    1 <文件头块>
        <标志符串>(4字节) + <头块数据区长度>(4字节) + <头块数据区>(6字节)
        eg. 4D 54 68 64 00 00 00 06 00 01 00 03 01 E0
        标志符串： 4D 54 68 64  MThd
        头块数据区长度: 00 00 00 06
        头块数据区: 00 01 00 03 01 E0
    1.1 头块数据区()
        共有6字节，分别为ff ff nn nn dd dd
    
        ff ff :
            00 00 表示只含一个音轨
            00 01 表示含有多个同步音轨
            00 10 表示含有多个独立音轨
        nn nn :
            指定轨道数，一般都会大于1，因为除了演播主音轨外，还会有全局音轨
        dd dd :
            指定基本时间格式，dd dd 的最高位为标记位，0为采用ticks计时，后面的数据为一个4分音符的ticks；
            1为SMPTE格式计时，后面的数值则是定义每秒中SMTPE帧的数量及每个SMTPE帧的tick。
            用我们举例的midi来看看，dd dd 的数据为01 E0，表示采用ticks计时，1E0转十进制为480，也就是每个4分音符，包含480ticks。
            后面事件时间都是以ticks为单位。

    2 <音轨块数据>
        <标志符串>(4字节) + <音轨块数据区长度>(4字节) + <音轨块数据区>(多个MIDI事件构成)

    2.1 <标志符串>
        eg. 4d 54 72 6b MTrk
    2.2 <音轨块数据区长度>
        指定后面的数据区长度: 不包换标志符串及音轨块数据区长度在内的后面数据字节数。
    2.2 <音轨块数据区>(多个MIDI事件构成)
        <MIDI事件>:
            <delta time> + <MIDI 消息>/<非MIDI事件>
    2.2.1 <MIDI 消息>
        其中delta time 就是采用动态字节来表示，单位就是tick。
        MIDI 消息，由一个状态字节 + 多个数据字节 构成。
        状态字节可以理解为方法，数据字节可以理解为这个方法的参数。
        状态字节的最高位永远为1，因为它的范围介于128~ 255之间，而数据字节最高位永远为0，所以的它的范围介于0 ~ 127 之间。
        消息根据性质可分成通道消息和系统消息两大类。
        通道消息是对单一的MIDI Channel起作用，其Channel是利用状态字节的低 4 位来表示，从0~F共有16个。
        下表为通道消息的同类，其中X为0~16.

            状态字节	功能描述	数据字节描述
            8X	松开音符	1字节:音符号(00~7F) / 2字节:力度(00~7F)
            9X	按下音符	1字节:音符号(00~7F) / 2字节:力度(00~7F)
            AX	触后音符	1字节:音符号(00~7F) / 2字节:力度(00~7F)
            BX	控制器变化	1字节:控制器号码(00~79) / 2字节:控制器参数(00~7F)
            CX	改变乐器	1字节:乐器号码(00~7F)
            DX	通道触动压力	1字节:压力(00~7F)
            EX	弯音轮变换	1字节:弯音轮变换值的低字节 / 2字节:弯音轮变换值的高字节

        eg. 按下音符
            8F 00 90 3C 64
            释放音符
            8C 18 80 3C 40
        
    2.2.2 <非MIDI事件>(也叫元事件)
        还有一种特殊的状态字节FF,表示非MIDI事件(Non- MIDI events)，也叫meta-event（元事件）。元事件的语法定于如下：
        
            FF + <种类字节>(1字节) + <数据字节长度> + <数据字节>

            FF的部分功能，其他如果数据字节数不是固定，而是有前面的动态字节制定，则用--表示

                种类	功能描述	数据字节长度	数据字节描述
                00	设置轨道音序	2	音序号 00 00-FF FF
                01	文字事件	--	文本信息
                02	版权公告	--	版权信息
                03	指定歌曲/音轨的名称	--	歌曲名称(用于全局音轨时)/音轨的名称
                04	指定乐器	--	乐器名称
                05	歌词	--	歌词
                06	标记	--	标记(通常在一个格式0的音轨，或在格式1的第一个音轨。)
                07	注释	--	描述一些在这一点上发生的动作或事件
                2F	音轨终止	--	音轨结束标志(必须有的)
                51	指定速度	--	设定速度，以微妙为单位，是四分音符的时值
                58	指定节拍	--	略

        eg. 指定歌曲名称
                00 FF 03 03 31 32 33
    2.2.3 音轨结束事件
        每个音轨最后肯定是以00 FF 2F 00结束，因为这是一个音轨结束事件
        

*/

#include "stdio.h"
#include "iostream"
#include "string.h"
#include "list"

#include "../../common/include/typedef.h"
#include "../../single/include/single.h"

using namespace std;

#define MIDI_FILENAME_SIZE  64


    //文件头块
    struct FileHeaderBlock_t
    {
        unsigned char headerFlag[4];
        unsigned long int len;
        struct 
        {
            unsigned short int ffff;        //音轨类型
            unsigned short int nnnn;        //音轨数
            unsigned short int dddd;        //计时类型及时值
        }data;        
    };
    typedef struct FileHeaderBlock_t FileHeaderBlock_t;

    //音轨数据头块
    struct AudioTrackHeader_t
    {
        unsigned char headerFlag[4];
        unsigned long int len;        
    };
    typedef struct AudioTrackHeader_t AudioTrackHeader_t;

    //Midi 事件
    struct MidiEvent_t
    {
        unsigned char code;             //操作码
        unsigned char syllable;         //音符
        unsigned char strength;         //力度
    };
    typedef struct MidiEvent_t MidiEvent_t;

    //非Midi 事件，也叫元事件
    struct MetaEvent_t
    {
        unsigned char flag;
        unsigned char code;
        unsigned char len;
        unsigned char *pData;
    };
    typedef struct MetaEvent_t MetaEvent_t;

    struct AudioTrackEnd_t
    {
        unsigned char flag;         //0xFF
        unsigned char code;         //0x2F
        unsigned char len;          //0x00
    };
    typedef struct AudioTrackEnd_t AudioTrackEnd_t;
    
    struct AudioTrackBlock_t
    {
        AudioTrackHeader_t audioTrackHeader;
        unsigned int delayTime;
        MidiEvent_t midiEvent;
        MetaEvent_t metaEvent;
    };
    typedef struct AudioTrackBlock_t AudioTrackBlock_t;

class CBlock
{
private:
    /* data */
public:
    CBlock(/* args */);
    ~CBlock();

    Err_t write();

    unsigned char * pData;
};

class CAudioTrackBlock : public CBlock
{
private:
    /* data */
public:
    CAudioTrackBlock(/* args */);
    ~CAudioTrackBlock();

    AudioTrackBlock_t audioTrackBlock;    
};

CAudioTrackBlock::CAudioTrackBlock(/* args */)
{
}

CAudioTrackBlock::~CAudioTrackBlock()
{
}




class CMidi
{
    
public:
    CMidi();    
    ~CMidi();

    Err_t open() ;
    Err_t close();

    Err_t Read(char *buff);
    Err_t Write(FileHeaderBlock_t fileHeaderBlock);
    Err_t Write(AudioTrackHeader_t audioTrackHeader);
    Err_t Write(MidiEvent_t midiEvent);
    Err_t Write(MetaEvent_t metaEvent);
    Err_t Write(AudioTrackEnd_t audioTrackEnd);
    Err_t WriteAudioTrackEnd();

//    Err_t Syllable2Midi(Syllable_t syl);

    int CalculateAudioTrackDataLength();

public:
    char name[MIDI_FILENAME_SIZE];
    char *fname;

    FILE *fp;

    
    FileHeaderBlock_t fileHeaderBlock;
    AudioTrackBlock_t audioTrackBlock;
    // AudioTrackHeader_t audioTrackHeader;
    // MidiEvent_t midiEvent;
    // MetaEvent_t metaEvent;
    // AudioTrackEnd_t audioTrackEnd;

    list<CSingle *> singleList;

private:

};

#endif

