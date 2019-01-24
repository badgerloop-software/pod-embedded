const constants = require('../../constants');
const events = require('events');
const dgram = require('dgram');
const net = require('net');

var udpServer = dgram.createSocket('udp4');
var PORT = constants.serverAddr.port;
var HOST = "127.0.0.1";
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

//TCP Packet Sending
const tcpServer = net.createServer((c) =>{
    console.log("Client connected");
    c.on('end', () => {
        console.log("client disconnected");
    });
    c.write('hello\r\n');
    c.pipe(c);
});

tcpServer.on('error', (err) => {
    throw err;
});

tcpServer.listen()



