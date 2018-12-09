const constants = require('../../constants');
const events = require('events');
const request = require('request');
var PORT = constants.serverAddr.port;
var HOST = constants.serverAddr.ip;
const recievedEmitter = new events.EventEmitter();
module.exports.recievedEmitter = recievedEmitter;
var dgram = require('dgram');
var server = dgram.createSocket('udp4');
var inData;

server.on('listening', function () {
    var address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

server.on('message', function (message, remote) {
    recieved = JSON.parse(message);
    switch (recieved.type) {
        case 'data':
            inData = recieved;
            module.exports.inData = inData;
            //Emit to proto.js that data has been recieved
            recievedEmitter.emit('dataIn');
            break;
        case 'disconnect':
            recievedEmitter.emit('disconnect'[recieved.subsystem]);
            break;
    }
});

recievedEmitter.on('heartbeat', function () {
    console.log("pong");
    // dashboard.changeState("podConnect", true);
});


module.exports.inData;
server.bind(PORT, HOST);


