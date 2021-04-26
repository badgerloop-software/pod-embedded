#ifndef BUART_H
#define BUART_H




#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <termios.h>
#include <string.h>
// File control definitions
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include "packet.h"


/*! To avoid unused parameters */
#define UNUSED(x) (void)(x)


/*!  \class     buart
     \brief     This class is used for communication over a serial device.
*/
class buart
{
public:

    //_____________________________________
    // ::: Constructors and destructors :::



    // Constructor of the class
    buart    ();

    // Destructor
    ~buart   ();



    //_________________________________________
    // ::: Configuration and initialization :::


    // Open a device
    char    openDevice (int devNum, const unsigned int Bauds);

    // Close the current device
    void    closeDevice();




    //___________________________________________
    // ::: Read/Write operation on characters :::


    // Write a char
    char    writeChar   (char);

    // Read a char (with timeout)
    char    readChar    (char *pByte,const unsigned int timeOut_ms=0);




    //________________________________________
    // ::: Read/Write operation on strings :::


    // Write a string
    char    writeString (const char *String);

    // Write a command
    char    writeCommand (BPacket *command);

    // Read a string (with timeout)
    int     readString  (   char *receivedString,
                            unsigned int maxNbBytes,
                            const unsigned int timeOut_ms=0);



    // _____________________________________
    // ::: Read/Write operation on bytes :::


    // Write an array of bytes
    char    writeBytes  (const void *Buffer, const unsigned int NbBytes);

    // Read an array of byte (with timeout)
    int     readBytes   (void *buffer,unsigned int maxNbBytes,const unsigned int timeOut_ms=0, unsigned int sleepDuration_us=100);




    // _________________________
    // ::: Special operation :::


    // Empty the received buffer
    char    flushReceiver();

    // Return the number of bytes in the received buffer
    int     available();

private:
    // Read a string (no timeout)
    int             readStringNoTimeOut  (char *String,unsigned int MaxNbBytes);

    // Current DTR and RTS state (can't be read on WIndows)
    bool            currentStateRTS;
    bool            currentStateDTR;

    int             fd;
};



/*!  \class     timeOut
     \brief     This class can manage a timer which is used as a timeout.
   */
// Class timeOut
class timeOut
{
public:

    // Constructor
    timeOut();

    // Init the timer
    void                initTimer();

    // Return the elapsed time since initialization
    unsigned long int   elapsedTime_ms();

private:
    // Used to store the previous time (for computing timeout)
    struct timeval      previousTime;
};

#endif // buart_H
