# Drivers
*Developers: Ezra Boley, Rohan Daruwala*

### Platform:
BeagleBone Black


## CAN API
This has two main functions. It reads and it writes to CAN. Before either functionality may be used, `init_can()` must be called.
    
First, reading is triggered on a timer by a state variable the timer toggles. 
This uses the underlying SIGALRM, and will be signaled at a fixed interval. The API expects that the control loop of the code that  utilizes it will then check that state variable, and call `read_can_message`. 
`read_can_message` takes in a `can_frame` type that will be used to return the data read in. The caller should then set the state variable to false, allowing for the timer to trigger the next read. 
    
Second, writing to CAN is done simply with a call to `send_can_msg`. It takaes in the ID of the message to send, an array of up to 8 bytes of data, and the number of bytes that should be sent. 

## BeagleBone GPIO

### Summary

The BeagleBone Black GPIO interface is exposed via the sysfs, and that
    interface is made easily accessible via this API.

### API

```C
        int bbGetAbsPinNum(unsigned int bank,
                unsigned int num);              /* There are different naming conventions, this converts
                                                   the bank_num to just an abs value */
        int bbGpioExport(unsigned int gpio);    /* Creates a sysfs entry for the
                                                   gpio pin */
        int bbGpioSetEdge(unsigned int gpio,
                char * edge);                   /* Enables interrupts on an edge for a pin (RISING_EDGE,
                                                   FALLING EDGE, or BOTH_EDGE) */
        int bbGpioSetDir(unsigned int gpio,
                char * dir);                    /* Sets input or output (IN_DIR
                                                   or OUT_DIR) */
        int bbGpioSetValue(unsigned int gpio,
                bool val);                      /* Sets value on a pin */

        int bbGpioFdOpen(unsigned int gpio);    /* Gets the FD for a gpio pins
                                                   value file */
        int bbGpioFdClose(int fd);              /* closes a FD */

        /* There are also getter functions, however their functionality is just
           the reverse of all the setters above */
```

### Known Issues

There are informal plans to move that to a faster system in the future as
sysfs only allows for toggling speeds of up to ~3.5 kHz, whereas other
methods (e.g. PRUs) can toggle close to 200 MHz.
