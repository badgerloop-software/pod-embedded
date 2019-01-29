# TCP Server Telemetry Loop
*Developer: Rohan Daruwala*

### Platform: 
	BeagleBone Black
	
### How it works:
	
Start the telemetry loop by calling the function:
	
```
#include src/Low_Voltage/LVTCPSocket.h
SetupLVTCPServer();
```

or

```
#include src/High_Voltage/HVTCPSocket.h
SetupHVTCPServer();
```

If you need to edit the port of the receiving server, modify the value in the respective .h files
	
### How to add commands

Add functionality by adding if statements to the .cpp file

```
if(!strncmp(buffer, "do something", MAX_COMMAND_SIZE)){
	// Do something
}
```

Change "do something" to the string command you want to receive. 

You may additionally choose to send a string back to the sending client by using the following commands:

```
char *toSend = "Hello world!"
send(new_socket , hello , strlen(toSend) , 0); 
```