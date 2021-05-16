
#include "../include/midi.h"


// CBlock::CBlock(/* args */)
// {
// }

// CBlock::~CBlock()
// {
// }


CMidi::CMidi()
{

}

CMidi::~CMidi()
{

}

Err_t CMidi::open()
{
//    fname = "./output/aa.mid";
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
    fclose(fp);

    return Err_OK;
}

Err_t CMidi::Read(char *buff)
{


    return Err_OK;
}

unsigned long int InvertByte(unsigned long int val)
{
    unsigned char tmp;
    dw2b_t dw2bTmp;

    dw2bTmp.dw = val;
    tmp = dw2bTmp.hsb;
    dw2bTmp.hsb = dw2bTmp.lsb;
    dw2bTmp.lsb = tmp;
    tmp = dw2bTmp.mhsb;
    dw2bTmp.mhsb = dw2bTmp.mlsb;
    dw2bTmp.mlsb = tmp;

    return dw2bTmp.dw;    
}

Err_t CMidi::Write(FileHeaderBlock_t fileHeaderBlock)
{
    Err_t err = Err_OK;

    int res = 0;


    memcpy(fileHeaderBlock.headerFlag,"MThd",4);
    res = fwrite(fileHeaderBlock.headerFlag,4,1,fp);

    fileHeaderBlock.len = 6;
    unsigned long int u32Tmp;
    u32Tmp = InvertByte(fileHeaderBlock.len);
    res = fwrite(&u32Tmp,4,1,fp);

    w2b_t w2bTmp;

    w2bTmp.w = fileHeaderBlock.data.ffff;    
    res = fwrite(&w2bTmp.hsb,1,1,fp);
    res = fwrite(&w2bTmp.lsb,1,1,fp);

    w2bTmp.w = fileHeaderBlock.data.nnnn;    
    res = fwrite(&w2bTmp.hsb,1,1,fp);
    res = fwrite(&w2bTmp.lsb,1,1,fp);

    w2bTmp.w = fileHeaderBlock.data.dddd;    
    res = fwrite(&w2bTmp.hsb,1,1,fp);
    res = fwrite(&w2bTmp.lsb,1,1,fp);

    return err;

}

Err_t CMidi::Write(AudioTrackHeader_t audioTrackHeader)
{
    int res = 0;

    // memcpy(audioTrackHeader.headerFlag,"MTrk",4);
    // res = fwrite(audioTrackHeader.headerFlag,4,1,fp);
    res = fwrite("MTrk",4,1,fp);     

    unsigned long int u32Tmp;
    u32Tmp = InvertByte(audioTrackHeader.len);
    res = fwrite(&u32Tmp,4,1,fp);

//    res = fwrite(&audioTrackHeader,sizeof(AudioTrackHeader_t),1,fp);     //长度需要后续再确定
    // if(res != sizeof(AudioTrackHeader_t))
    //     return Err_WriteFault;

    return Err_OK;
}

Err_t CMidi::Write(MidiEvent_t midiEvent)
{
    int res = 0;

    w2b_t w2bTmp;
    w2bTmp.w = midiEvent.dt;
    //write dt
    if(w2bTmp.hsb != 0)
        res = fwrite(&w2bTmp.hsb,1,1,fp);
    res = fwrite(&w2bTmp.lsb,1,1,fp);
    //write code
    res = fwrite(&midiEvent.code,1,1,fp);
    //write data
    res = fwrite(&midiEvent.data1,1,1,fp);
    if(midiEvent.data2 != 0)
        res = fwrite(&midiEvent.data2,1,1,fp);


    return Err_OK;
}

Err_t CMidi::WriteSingle()
{
    for(list<CSingle *>::iterator it = singleList.begin();it != singleList.end();++it)
    {    
        fwrite((*it)->buff_midi,(*it)->count,1,fp);
    }
    return Err_OK;

}

Err_t CMidi::Write(MetaEvent_t metaEvent)
{
    int res = 0;

    if(metaEvent.len != 0)
        if(metaEvent.pData == NULL)
            return Err_MetaEventData;

    w2b_t w2bTmp;
    w2bTmp.w = metaEvent.dt;

    //write dt
    if(w2bTmp.hsb != 0)
        res = fwrite(&w2bTmp.hsb,1,1,fp);
    res = fwrite(&w2bTmp.lsb,1,1,fp);
    //write flag
    res = fwrite(&metaEvent.flag,1,1,fp);
    //write code
    res = fwrite(&metaEvent.code,1,1,fp);
    //write len & data
    if(metaEvent.len != 0)
        res = fwrite(&metaEvent.pData,metaEvent.len,1,fp);
    else
        res = fwrite(&metaEvent.len,1,1,fp);


    return Err_OK;
}

Err_t CMidi::Write(AudioTrackEnd_t audioTrackEnd)
{


    return Err_OK;
}

// Err_t CMidi::WriteMetaEvent()
// {
//     w2b_t w2bTmp;
//     w2bTmp.w = metaEvent.dt;
//     if(w2bTmp.hsb != 0)
//         res = fwrite(&w2bTmp.hsb,1,1,fp);

//     res = fwrite(&w2bTmp.lsb,1,1,fp);
//     res = fwrite(&metaEvent.flag,1,1,fp);
//     res = fwrite(&metaEvent.code,1,1,fp);

//     if(metaEvent.len != 0)
//         res = fwrite(&metaEvent.pData,metaEvent.len,1,fp);
//     else
//         res = fwrite(&metaEvent.len,1,1,fp);


// //    res = fwrite(&metaEvent,sizeof(metaEvent),1,fp);     //长度需要后续再确定
//     if(res != sizeof(AudioTrackEnd_t))
//         return Err_WriteFault;

//     return Err_OK;
// }

Err_t CMidi::WriteAudioTrackEnd()
{
    int res = 0;

    AudioTrackEnd_t audioTrackEnd;

    audioTrackEnd.dt = 0x00;
    audioTrackEnd.flag = 0xFF;
    audioTrackEnd.code = 0x2F;
    audioTrackEnd.len = 0x00;

    res = fwrite(&audioTrackEnd,sizeof(AudioTrackEnd_t),1,fp);     //长度需要后续再确定
    if(res != sizeof(AudioTrackEnd_t))
        return Err_WriteFault;

    return Err_OK;
}


