const constants = require('../../constants');
const events = require('events');
var dgram = require('dgram');
var server = dgram.createSocket('udp4');
var PORT = constants.serverAddr.port;
var HOST = constants.serverAddr.ip;
const recievedEmitter = new events.EventEmitter() ;
module.exports.recievedEmitter = recievedEmitter;
var inData;

server.on('listening', function () {
    var address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

server.on('message', function (message, remote) {
    recieved = JSON.parse(message);
    switch (recieved.type) {
        case 'data':
            module.exports.inData = recieved;
            //Emit to handler.js that data has been recieved
            recievedEmitter.emit('dataIn');
            break;
        case 'disconnect':
            recievedEmitter.emit('disconnect'[recieved.subsystem]);
            break;
    }
});

recievedEmitter.on('heartbeat', function () {
    console.log("pong");
});


module.exports.inData;
server.bind(PORT, HOST);


