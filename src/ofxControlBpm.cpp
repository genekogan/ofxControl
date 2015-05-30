#include "ofxControlBpm.h"

ofxControlBpm::ofxControlBpm(float bpm,int beatPerBar) : beatPerBar(beatPerBar)
{
    isPlaying = false;
    isTick = false;
    setBpm(bpm);
}

void ofxControlBpm::start()
{
    stop();
    isPlaying = true;
    reset();
    startThread(true, false);
}

void ofxControlBpm::stop()
{
    waitForThread(true);
    isPlaying = false;
    stopThread();
}

void ofxControlBpm::reset()
{
    if(lock())
    {
        
        remainderOfTick = 0;
        countOfTick = -1;
        preTime = ofGetElapsedTimeMicros();
        totalTime = 0;
        
        unlock();
    }
}

void ofxControlBpm::threadedFunction()
{
    while (isThreadRunning() != 0)
    {
        if (lock())
        {
            if (isPlaying)
            {
                long nowTime = ofGetElapsedTimeMicros();
                long deltaTime = nowTime - preTime;
                preTime = nowTime;
                
                if (totalTime + remainderOfTick >= durationOfTick)
                {
                    if ((countOfTick % (BPM_TICK / beatPerBar)) == 0)
                    {
                        ofNotifyEvent(beatEvent);
                    }
                    
                    remainderOfTick = (totalTime + remainderOfTick) % durationOfTick;
                    totalTime = 0.;
                    isTick = true;
                    
                    countOfTick++;
                    countOfTick %= BPM_TICK;
                }
                else
                {
                    isTick = false;
                }
                
                totalTime += deltaTime;
                
                unlock();
                yield();
            }
        }
    }
}

void ofxControlBpm::setBeatPerBar(int beatPerBar)
{
    if (lock())
    {
        beatPerBar = beatPerBar;
        unlock();
    }
}

void ofxControlBpm::setBpm(float bpm)
{
    if(lock())
    {
        this->bpm = bpm;
        durationOfTick = 60. * 1000. * 1000. / (bpm * (BPM_TICK >> 2));
        unlock();
    }
}

float ofxControlBpm::getBpm() const
{
    return bpm;
}

bool ofxControlBpm::getPlaying() const
{
    return isPlaying;
}
