const constants = require('./constants');
const DATA_SEND_RATE = 300;
const dgram = require('dgram');
const IP = constants.serverAddr.ip, PORT = constants.serverAddr.port; 

/* We're gonna come up with a damn template for sending data from the server to the dashboard one day
*
*
* I think today is that day
*/
var client = dgram.createSocket('udp4');

// From dgram docs
// client.send(message, 0, message.length, PORT, HOST, function(err, bytes) {
//     if (err) throw err;
//     console.log('UDP message sent to ' + HOST +':'+ PORT);
//     client.close();
// });
// heartbeat();

function heartbeat(){
    client.send("ping", 0, "ping".length, PORT, IP, function(err,bytes){
        if(err) throw err;
        console.log('ping');
    })
}
function sendJSON(object){
    console.log("send data");
    sendData(JSON.stringify(object));
}

function sendTestData(){
    let testSocket = {
        type: 'data',
        data:{
            motion : {
                stoppingDistance: getRandomValue(),
                position: getRandomValue(),
                retro: getRandomValue(),
                velocity: getRandomValue(),
                acceleration: getRandomValue()
            },
            battery: {
                packVoltage: getRandomValue(),
                packCurrent: getRandomValue(),
                packSOC: getRandomValue(),
                packAH: getRandomValue(),
                cellMaxVoltage: getRandomValue(),
                cellMinVoltage: getRandomValue(),
                highTemp: getRandomValue(),
                lowTemp: getRandomValue()
            },
            braking: {
                secondaryTank: getRandomValue(),
                secondaryLine: getRandomValue(),
                secondaryActuation: getRandomValue(),
                primaryTank: getRandomValue(),
                primaryLine: getRandomValue(),
                primaryActuation: getRandomValue(),
                pressureVesselPressure: getRandomValue(),
                currentPressure: getRandomValue()
            }

        }
   }
    sendJSON(testSocket);
}

//The line where test data is sent. setInterval(function, ms)
// 

setInterval(sendTestData, DATA_SEND_RATE);

function sendData(data){
    client.send(data,0,data.length, PORT, IP, function(err, bytes){
        if(err) throw err;
    })
}

function getRandomIntInclusive(min, max) {
    min = Math.ceil(min);
    max = Math.floor(max);
    return Math.floor(Math.random() * (max - min + 1)) + min; //The maximum is inclusive and the minimum is inclusive 
  }

  function getRandomValue(){
     return getRandomIntInclusive(0,255);
  }