# Dashboard Server Telemetry Loop
*Developer: Rohan Daruwala*

### Platform: 
	BeagleBone Black
	
### How it works:
	
Start the telemetry loop by calling the function:
	
```
#include src/Low_Voltage/LVTelemetry.h
SetupLVTelemetry();
```

or

```
#include src/High_Voltage/HVTelemetry.h
SetupHVTelemetry();
```

If you need to edit the IP address or port of the receiving server, modify the values in the respective .h files

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