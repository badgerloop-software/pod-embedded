# CAN API

This has two main functions. It reads and it writes to CAN. Before either functionality may be used, `init_can()` must be called.
    
    First, reading is triggered on a timer by a state variable the timer toggles. 
This uses the underlying SIGALRM, and will be signaled at a fixed interval. The API expects that the control loop of the code that utilizes it will then check that state variable, and call `read_can_message`. `read_can_message` takes in a `can_frame` type that will be used to return the data read in. 
The caller should then set the state variable to false, allowing for the timer to trigger the next read. 

    Second, writing to CAN is done simply with a call to `send_can_msg`. It takaes in the ID of the message to send, an array of up to 8 bytes of data, and the number of bytes that should be sent. 

    `can_test.c` in the embedded/tests/ is a good example of how the API can be utilized.
