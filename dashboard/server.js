var constants = require('./constants');
var dgram = require('dgram');

const ip = constants.serverAddr.ip, port = constants.serverAddr.port;

var client = dgram.createSocket('udp4');
// client.send(message, 0, message.length, PORT, HOST, function(err, bytes) {
//     if (err) throw err;
//     console.log('UDP message sent to ' + HOST +':'+ PORT);
//     client.close();
// });


function heartbeat(){
    client.send("ping", 0, "ping".length, port, ip, function(err,bytes){
        if(err) throw err;
        console.log('ping');
    })
}

module.exports.heartbeat