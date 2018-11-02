var constants = require('./constants');
var dgram = require('dgram');

const IP = constants.serverAddr.ip, PORT = constants.serverAddr.port; 

/* We're gonna come up with a damn template for sending data from the server to the dashboard one day
*
*
* Today is not that day 
*/

// var testSocket = {
//     age: new Date().getUTCMilliseconds(),
//     data: {
//      navigation : {
//          imu: getRandomValue(),
//          encoder: getRandomValue(),
//          retro: getRandomValue() 
//      },
//      pressureVessel: {
//          pressure: getRandomValue(),
//          airQuality: getRandomValue(),
//          paranormal: getRandomValue()
//      },
//      braking: {
//          solenoids: getRandomValue(),
//          brakePadTemps: getRandomValue(),
//          brakeGood: getRandomValue()
//      }
//     }
// }
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
    sendData(JSON.stringify(object));
}

function sendTestData(){
    let testSocket = {
        age: Date.now(),
        data:{
            navigation : {
                imu: getRandomValue(),
                encoder: getRandomValue(),
                retro: getRandomValue() 
            },
            pressureVessel: {
                pressure: getRandomValue(),
                airQuality: getRandomValue(),
                paranormal: getRandomValue()
            },
            braking: {
                solenoids: getRandomValue(),
                brakePadTemps: getRandomValue(),
                brakeGood: getRandomValue()
            }

        }
   }
    sendJSON(testSocket);
}


setInterval(sendTestData, 1);

function sendData(data){
    client.send(data,0,data.length, PORT, IP, function(err, bytes){
        if(err) throw err;
        console.log('Send Data');
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