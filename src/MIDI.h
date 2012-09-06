/*!
 *  @file       MIDI.h
 *  Project     Arduino MIDI Library
 *  @brief      MIDI Library for the Arduino
 *  @version    4.0
 *  @author     Francois Best 
 *  @date       24/02/11
 *  license     GPL Forty Seven Effects - 2011
 */

#pragma once

#include "midi_Settings.h"
#include "midi_Defs.h"

#ifdef FSE_AVR
#   include "hardware_Serial.h"
#else
#   include "Arduino.h"
#endif

// -----------------------------------------------------------------------------

BEGIN_MIDI_NAMESPACE

/*! \brief The main class for MIDI handling.
 */
class MidiInterface
{
public:
    MidiInterface();
    ~MidiInterface();
    
public:
    void begin(Channel inChannel = 1);
    
    // -------------------------------------------------------------------------
    // MIDI Output
    
#if MIDI_BUILD_OUTPUT
    
public:
    inline void sendNoteOn(DataByte inNoteNumber,
                           DataByte inVelocity,
                           Channel inChannel);
    
    inline void sendNoteOff(DataByte inNoteNumber,
                            DataByte inVelocity,
                            Channel inChannel);
    
    inline void sendProgramChange(DataByte inProgramNumber,
                                  Channel inChannel);
    
    inline void sendControlChange(DataByte inControlNumber,
                                  DataByte inControlValue, 
                                  Channel inChannel);
    
    inline void sendPitchBend(int inPitchValue,    Channel inChannel);
    inline void sendPitchBend(double inPitchValue, Channel inChannel);
    
    inline void sendPolyPressure(DataByte inNoteNumber,
                                 DataByte inPressure,
                                 Channel inChannel);
    
    inline void sendAfterTouch(DataByte inPressure,
                               Channel inChannel);
    
    inline void sendSysEx(unsigned int inLength, 
                          const byte* inArray,
                          bool inArrayContainsBoundaries = false);    
    
    inline void sendTimeCodeQuarterFrame(DataByte inTypeNibble, 
                                         DataByte inValuesNibble);
    inline void sendTimeCodeQuarterFrame(DataByte inData);
    
    inline void sendSongPosition(unsigned int inBeats);
    inline void sendSongSelect(DataByte inSongNumber);
    inline void sendTuneRequest();
    inline void sendRealTime(MidiType inType);
    
public:
    void send(MidiType inType,
              DataByte inData1,
              DataByte inData2,
              Channel inChannel);
    
private:
    inline StatusByte getStatus(MidiType inType,
                                Channel inChannel) const;
    
#endif // MIDI_BUILD_OUTPUT
    
    
    // -------------------------------------------------------------------------
    // MIDI Input
    
#if MIDI_BUILD_INPUT
    
public:
    bool read();
    bool read(Channel inChannel);
    
public:
    inline MidiType getType() const;
    inline Channel  getChannel() const;
    inline DataByte getData1() const;
    inline DataByte getData2() const;
    inline const byte* getSysExArray() const;
    inline unsigned int getSysExArrayLength() const;
    inline bool check() const;
    
public:
    inline Channel getInputChannel() const;
    inline void setInputChannel(Channel inChannel);
    
public:
    static inline MidiType getTypeFromStatusByte(const byte inStatus);
    
private:
    bool inputFilter(Channel inChannel);
    bool parse(Channel inChannel);
    void resetInput();
    
private:
    StatusByte mRunningStatus_RX;
    Channel    mInputChannel;
    
    byte         mPendingMessage[3];             // SysEx are dumped into mMessage directly.
    unsigned int mPendingMessageExpectedLenght;
    unsigned int mPendingMessageIndex;           // Extended to unsigned int for larger SysEx payloads.
    Message mMessage;
    
    
    // -------------------------------------------------------------------------
    // Input Callbacks
    
#if MIDI_USE_CALLBACKS
    
public:
    inline void setHandleNoteOff(void (*fptr)(byte channel, byte note, byte velocity));
    inline void setHandleNoteOn(void (*fptr)(byte channel, byte note, byte velocity));
    inline void setHandleAfterTouchPoly(void (*fptr)(byte channel, byte note, byte pressure));
    inline void setHandleControlChange(void (*fptr)(byte channel, byte number, byte value));
    inline void setHandleProgramChange(void (*fptr)(byte channel, byte number));
    inline void setHandleAfterTouchChannel(void (*fptr)(byte channel, byte pressure));
    inline void setHandlePitchBend(void (*fptr)(byte channel, int bend));
    inline void setHandleSystemExclusive(void (*fptr)(byte * array, byte size));
    inline void setHandleTimeCodeQuarterFrame(void (*fptr)(byte data));
    inline void setHandleSongPosition(void (*fptr)(unsigned int beats));
    inline void setHandleSongSelect(void (*fptr)(byte songnumber));
    inline void setHandleTuneRequest(void (*fptr)(void));
    inline void setHandleClock(void (*fptr)(void));
    inline void setHandleStart(void (*fptr)(void));
    inline void setHandleContinue(void (*fptr)(void));
    inline void setHandleStop(void (*fptr)(void));
    inline void setHandleActiveSensing(void (*fptr)(void));
    inline void setHandleSystemReset(void (*fptr)(void));
    
    inline void disconnectCallbackFromType(MidiType inType);
    
private:
    
    void launchCallback();
    
    void (*mNoteOffCallback)(byte channel, byte note, byte velocity);
    void (*mNoteOnCallback)(byte channel, byte note, byte velocity);
    void (*mAfterTouchPolyCallback)(byte channel, byte note, byte velocity);
    void (*mControlChangeCallback)(byte channel, byte, byte);
    void (*mProgramChangeCallback)(byte channel, byte);
    void (*mAfterTouchChannelCallback)(byte channel, byte);
    void (*mPitchBendCallback)(byte channel, int);
    void (*mSystemExclusiveCallback)(byte * array, byte size);
    void (*mTimeCodeQuarterFrameCallback)(byte data);
    void (*mSongPositionCallback)(unsigned int beats);
    void (*mSongSelectCallback)(byte songnumber);
    void (*mTuneRequestCallback)(void);
    void (*mClockCallback)(void);
    void (*mStartCallback)(void);
    void (*mContinueCallback)(void);
    void (*mStopCallback)(void);
    void (*mActiveSensingCallback)(void);
    void (*mSystemResetCallback)(void);
    
#endif // MIDI_USE_CALLBACKS    
    
#endif // MIDI_BUILD_INPUT
    
    
    // -------------------------------------------------------------------------
    // MIDI Soft Thru
    
#if MIDI_BUILD_THRU
    
public:
    inline MidiFilterMode getFilterMode() const;
    inline bool getThruState() const;
    
    inline void turnThruOn(MidiFilterMode inThruFilterMode = Full);
    inline void turnThruOff();
    inline void setThruFilterMode(MidiFilterMode inThruFilterMode);
    
    
private:
    void thruFilter(byte inChannel);
    
private:
    bool            mThruActivated  : 1;
    MidiFilterMode  mThruFilterMode : 7;
    
#endif // MIDI_BUILD_THRU
    

#if MIDI_USE_RUNNING_STATUS
    
private:
    StatusByte mRunningStatus_TX;
    
#endif // MIDI_USE_RUNNING_STATUS
    
};

END_MIDI_NAMESPACE

// -----------------------------------------------------------------------------

#if MIDI_AUTO_INSTANCIATE
    extern MIDI_NAMESPACE::MidiInterface MIDI;
#endif // MIDI_AUTO_INSTANCIATE

// -----------------------------------------------------------------------------

#include "midi_Inline.hpp"
