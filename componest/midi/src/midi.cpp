
#include "../include/midi.h"


CBlock::CBlock(/* args */)
{
}

CBlock::~CBlock()
{
}


CMidi::CMidi()
{

}

CMidi::~CMidi()
{

}

Err_t CMidi::open()
{
    fname = "./test/aa.txt";
    fp = fopen(fname,"wb");
    if(fp == NULL)
    {
        cout<<fname<<"is not available"<<endl;
        return Err_FileOpenFailed;
    }

    return Err_OK;
}

Err_t CMidi::close()
{

    return Err_OK;
}

Err_t CMidi::Read(char *buff)
{


    return Err_OK;
}

Err_t CMidi::Write(FileHeaderBlock_t fileHeaderBlock)
{
    Err_t err = Err_OK;

    int res = 0;


    memcpy(fileHeaderBlock.headerFlag,"MThd",4);
    fileHeaderBlock.len = 6;
    // fileHeaderBlock.data.ffff = 0x0001;
    // fileHeaderBlock.data.nnnn = 0x0006;
    // fileHeaderBlock.data.dddd = 0x01E0;

    res = fwrite(&fileHeaderBlock,sizeof(FileHeaderBlock_t),1,fp);
    if(res != sizeof(FileHeaderBlock_t))
    {
        err = Err_WriteFault;
        return err;
    }



//    fclose(fp);

    return err;

}

Err_t CMidi::Write(AudioTrackHeader_t audioTrackHeader)
{
    int res = 0;

    memcpy(audioTrackHeader.headerFlag,"MTrk",4);
    audioTrackHeader.len = 0;

    res = fwrite(&audioTrackHeader,sizeof(AudioTrackHeader_t),1,fp);     //长度需要后续再确定
    if(res != sizeof(AudioTrackHeader_t))
        return Err_WriteFault;

    return Err_OK;
}

Err_t CMidi::Write(MidiEvent_t midiEvent)
{
    Err_t err = Err_OK;


    return err;

}

Err_t CMidi::Write(MetaEvent_t metaEvent)
{

    return Err_OK;

}

Err_t CMidi::Write(AudioTrackEnd_t audioTrackEnd)
{


    return Err_OK;
}

Err_t CMidi::WriteAudioTrackEnd()
{
    int res = 0;

    AudioTrackEnd_t audioTrackEnd;

    audioTrackEnd.flag = 0xFF;
    audioTrackEnd.code = 0x2F;
    audioTrackEnd.len = 0x00;

    res = fwrite(&audioTrackEnd,sizeof(AudioTrackHeader_t),1,fp);     //长度需要后续再确定
    if(res != sizeof(AudioTrackEnd_t))
        return Err_WriteFault;

    return Err_OK;
}


