const constants = require('./constants');
const events = require('events');
const request = require('request');
const dashboard = require('./public/javascripts/proto');
var PORT = constants.serverAddr.port;
var HOST = constants.serverAddr.ip;
const recievedEmitter = new events.EventEmitter();
module.exports.recievedEmitter = recievedEmitter
var dgram = require('dgram');
var server = dgram.createSocket('udp4');

server.on('listening', function () {
    var address = server.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

server.on('message', function (message, remote) {
    console.log(remote.address + ':' + remote.port +' - ' + message);
    if(message === "ping"){
        recievedEmitter.emit("heartbeat");
    }

});

recievedEmitter.on('heartbeat', function(){
    console.log("pong");
    dashboard.changeState("podConnect", true);
})

server.bind(PORT, HOST);

