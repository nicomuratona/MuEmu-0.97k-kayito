#include "stdafx.h"
#include "Mp3.h"

Mp3 gMP3;

Mp3::Mp3()
{
        this->pigb = NULL;

        this->pimc = NULL;

        this->pimex = NULL;

        this->piba = NULL;

        this->pims = NULL;

        this->ready = false;

        this->duration = 0;
}

Mp3::~Mp3()
{

}

void Mp3::Cleanup()
{
        if (this->pimc)
        {
                this->pimc->Stop();
        }

        if (this->pigb)
        {
                this->pigb->Release();

                this->pigb = NULL;
        }

        if (this->pimc)
        {
                this->pimc->Release();

                this->pimc = NULL;
        }

        if (this->pimex)
        {
                this->pimex->Release();

                this->pimex = NULL;
        }

        if (this->piba)
        {
                this->piba->Release();

                this->piba = NULL;
        }

        if (this->pims)
        {
                this->pims->Release();

                this->pims = NULL;
        }

        this->ready = false;
}

bool Mp3::Load(LPCWSTR szFile)
{
        this->Cleanup();

        if (SUCCEEDED(CoCreateInstance(CLSID_FilterGraph,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_IGraphBuilder,
            (void**)&this->pigb)))
        {
                this->pigb->QueryInterface(IID_IMediaControl, (void**)&this->pimc);

                this->pigb->QueryInterface(IID_IMediaEventEx, (void**)&this->pimex);

                this->pigb->QueryInterface(IID_IBasicAudio, (void**)&this->piba);

                this->pigb->QueryInterface(IID_IMediaSeeking, (void**)&this->pims);

                HRESULT hr = this->pigb->RenderFile(szFile, NULL);

                if (SUCCEEDED(hr))
                {
                        this->ready = true;

                        if (this->pims)
                        {
                                this->pims->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);

                                this->pims->GetDuration(&this->duration); // returns 10,000,000 for a second.

                                this->duration = this->duration;
                        }
                }
        }

        return this->ready;
}

bool Mp3::Play()
{
        if (this->ready && this->pimc)
        {
                HRESULT hr = this->pimc->Run();

                return SUCCEEDED(hr);
        }

        return false;
}

bool Mp3::Pause()
{
        if (this->ready && this->pimc)
        {
                HRESULT hr = this->pimc->Pause();

                return SUCCEEDED(hr);
        }

        return false;
}

bool Mp3::Stop()
{
        if (this->ready && this->pimc)
        {
                HRESULT hr = this->pimc->Stop();

                return SUCCEEDED(hr);
        }

        return false;
}

bool Mp3::WaitForCompletion(long msTimeout, long* EvCode)
{
        if (this->ready && this->pimex)
        {
                HRESULT hr = this->pimex->WaitForCompletion(msTimeout, EvCode);

                return *EvCode > 0;
        }

        return false;
}

bool Mp3::SetVolume(long vol)
{
        if (this->ready && this->piba)
        {
                HRESULT hr = this->piba->put_Volume(vol);

                return SUCCEEDED(hr);
        }

        return false;
}

long Mp3::GetVolume()
{
        if (this->ready && this->piba)
        {
                long vol = -1;

                HRESULT hr = this->piba->get_Volume(&vol);

                if (SUCCEEDED(hr))
                {
                        return vol;
                }
        }

        return -1;
}

__int64 Mp3::GetDuration()
{
        return this->duration;
}

__int64 Mp3::GetCurrentPosition()
{
        if (this->ready && this->pims)
        {
                __int64 curpos = -1;

                HRESULT hr = this->pims->GetCurrentPosition(&curpos);

                if (SUCCEEDED(hr))
                {
                        return curpos;
                }
        }

        return -1;
}

bool Mp3::SetPositions(__int64* pCurrent, __int64* pStop, bool bAbsolutePositioning)
{
        if (this->ready && this->pims)
        {
                DWORD flags = 0;

                if (bAbsolutePositioning)
                {
                        flags = AM_SEEKING_AbsolutePositioning | AM_SEEKING_SeekToKeyFrame;
                }
                else
                {
                        flags = AM_SEEKING_RelativePositioning | AM_SEEKING_SeekToKeyFrame;
                }

                HRESULT hr = this->pims->SetPositions(pCurrent, flags, pStop, flags);

                if (SUCCEEDED(hr))
                {
                        return true;
                }
        }

        return false;
}