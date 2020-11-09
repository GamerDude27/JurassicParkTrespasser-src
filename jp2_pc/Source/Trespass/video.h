//+--------------------------------------------------------------------------
//
//  Copyright (C) DreamWorks Interactive, 1997.
//
//  File:       VIDEO.h
//
//  Contents:   Header file for Videos
//
//  Classes:
//
//  Functions:  
//
//  History:    08-Jul-98   SHernd   Created
//
//---------------------------------------------------------------------------


#ifndef __VIDEO_H__
#define __VIDEO_H__


#include "uiwnd.h"


struct smk_t;


typedef struct {
    unsigned char values[3];
} _ubytevec3;

struct VideoFrame
{
    std::vector<unsigned char> video = std::vector<unsigned char>(640 * 348);
    std::vector<_ubytevec3> videoPalette = std::vector<_ubytevec3>(256);
};


class CVideoWnd : public CUIWnd
{
public:
    CVideoWnd(CUIManager * puimgr);
    virtual ~CVideoWnd();

    BOOL Play(LPCSTR pszFile);
    void OnKey(UINT vk, BOOL fDown, int cRepeat, UINT flags);

    BOOL AllowEscape() { return FALSE; }

    void NextSmackerFrame();
    void NextImageFrame();

    bool CreateSoundBuffer();
    bool WriteChunkIntoAudioBuffer(DWORD& lastWriteOffset);
    bool ServiceAudioBuffer(bool force = false);
    void LoadFrameIntoQueues();

    void Pause();
    void Resume();

    char                m_szFile[_MAX_PATH];
    smk_t*              m_pSmack;
    std::list<VideoFrame> m_frameQueue;
    std::list<std::vector<char>> m_audioQueue;
    CCom<IDirectSoundBuffer> m_soundbuffer;

	std::unique_ptr<CRasterVid> m_pVideoFrameBuf;
    int                 m_iLastKey;
    int                 m_iTop;
    int                 m_iLeft;
    int                 m_framerate;
    bool                m_fVideoLoaded;
    bool                m_fVideoOver;
    int                 m_iSurfaceType;

    struct
    {
        DWORD           m_soundBufferSize;
        char            m_silenceByte;
        size_t          m_avgBytesPerSec;
        bool            m_audioFinished;
        bool            m_playingFirstHalf;
        bool            m_audioWriteFinished;
        DWORD           m_lastWriteOffset;
        DWORD           m_finalWriteOffset;
    }                   m_audioState;
};


#endif

