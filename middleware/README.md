# Pod Middleware
*Developer: Rohan Daruwala*

Everything that facilitates data-transfer between the Beaglebones on the pod, or between the pod and the dashboard

## Platform: 
	BeagleBone Black


## TCP Server Telemetry Loop

The TCP Server Telemetry Loop allows for commands to be sent from the dashboard to the beaglebone. This implementation is the Beaglebone end, where data is received and parsed.

### How it works:
	
Start the telemetry loop by calling the function:
	
```
#include "LVTCPSocket.h"
SetupLVTCPServer();
```

or

```
#include "HVTCPSocket.h"
SetupHVTCPServer();
```

If you need to edit the port of the receiving server, modify the value in the respective .h file
	
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
send(new_socket , toSend , strlen(toSend) , 0); 
```

## UDP Data Telemetry Loop

The UDP Telemetry Loop gathers data from the various sensors connected to the Beaglebone. It then generates a packet and sends it to the dashboard server.

### How it works:

Start the telemetry loop by calling the function:
	
```
#include "TelemetryLoop.h"
SetupTelemetry((char *) "IP Address", port);
```

Where "IP Address and "Port" are that of the dashboard server.

This feature uses the [Boost CRC](https://www.boost.org/doc/libs/1_71_0/doc/html/crc.html) library to generate a cyclical redundancy check.
	
### How to add data

In order to add more data, add additional fields to the `autocoder/data.xml`. The auto-coder will add the fields to the data.h tree structure. So long as the `data.xml` on the pod and on the dashboard are identical, the fields will be automatically sent.