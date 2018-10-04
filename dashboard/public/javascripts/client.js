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
    console.log(remote.address + ':' + remote.port + ' - ' + message);
    if (message == "ping") {
        recievedEmitter.emit("heartbeat");
    } else {
        
        inData = JSON.parse(message);
        //console.log(inData);
        module.exports.inData = inData;
        console.log(module.exports.inData)
        // console.log(module.exports.inData);
        recievedEmitter.emit('dataIn');
    }
});

recievedEmitter.on('heartbeat', function () {
    console.log("pong");
    // dashboard.changeState("podConnect", true);
});

module.exports.inData;
server.bind(PORT, HOST);


