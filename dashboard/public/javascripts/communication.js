const constants = require('../../constants');
const events = require('events');
const dgram = require('dgram');
const net = require('net');

var udpServer = dgram.createSocket('udp4');
var PORT = constants.serverAddr.port;
var HOST = constants.serverAddr.ip;
const recievedEmitter = new events.EventEmitter();
module.exports.recievedEmitter = recievedEmitter;
var inData;

//UDP Data Recieving

udpServer.on('listening', function () {
    var address = udpServer.address();
    console.log('UDP Server listening on ' + address.address + ":" + address.port);
});

udpServer.on('message', function (message, remote) {
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
udpServer.bind(PORT, HOST);

module.exports.sendPacket = function sendPacket(ip, port, msg){

let tcpSender = new net.Socket();
tcpSender.connect(port, ip, () =>{
    console.log('Pod Connected');
    tcpSender.write(msg);
});

tcpSender.setTimeout(2000);

tcpSender.on('data', (e)=>{
    console.log('Recieved: ' + e);
});

tcpSender.on('close', () =>{
    console.log('Connection Closed');
});
}