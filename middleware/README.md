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

The UDP Telemetry Loop gathers data from the various sensors connected to the beaglebones. It then generates a packet and sends it to the dashboard server.

### How it works:
	
Start the telemetry loop by calling the function:
	
```
#include "LVTelemetry_Loop.h"
SetupLVTelemetry("IP Address", Port);
```

or

```
#include "HVTelemetry_Loop.h"
SetupHVTelemetry("IP Address", Port);
```

Where "IP Address and "Port" are that of the dashboard server.

This feature uses the [RapidJSON](http://rapidjson.org) library to handle JSON objects with ease.
	
### How to add data

As functionality is added, you'll need to add data to the JSON string sent to the server. Several data points have already been added, based off of [this dashboard example](https://github.com/badgerloop-software/pod/blob/6953b71426e69a523f0ab82c737bd0ef032e486a/dashboard/server.js). Features that have not been implemented yet only return NULL values.

To add a data point, you'll first need to create a "Value" object. Next, set the value using the setXYZ command. You can use SetNull(), SetFloat(), SetInt(), etc.

```
Value newData;
newData.SetInt(42);
```

Next, you'll want to add it to a document (JSON object). To create one:

```
Document document;
document.SetObject();
```

To add a data point (or another document) to a document, use the following command:

```
document.AddMember("key", newData, document.GetAllocator());
```