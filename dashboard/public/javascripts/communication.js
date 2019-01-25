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


var tcpSender = new net.Socket();
tcpSender.connect(constants.serverAddr.tcpPort, constants.serverAddr.ip, () =>{
    console.log('Pod Connected');
    tcpSender.write('Pod, are you a beaver? Cause dam');
});

tcpSender.setTimeout(2000);

tcpSender.on('data', (e)=>{
    console.log('Recieved: ' + e);
    tcpSender.destroy();
});

tcpSender.on('close', () =>{
    console.log('Connection Closed');
});
/*

This is some stuff I want to play with later - Eric

*/

//TCP Packet Sending
// const tcpServer = net.createServer((c) =>{
//     console.log("Client connected Server " + c.remoteAddress+':'+c.remotePort);
//     c.on('end', () => {
//         console.log("client disconnected");
//     });
//     c.write('hello\r\n');
//     c.pipe(c);
// });

// tcpServer.on('error', (err) => {
//     throw err;
// });

// tcpServer.on('connection', (c) =>{
//     console.log(c);
// })

// tcpServer.listen(constants.serverAddr.tcpPort, constants.serverAddr.ip);
// function getConnection(){
// const options = {
//     host: constants.serverAddr.ip,
//     port: constants.serverAddr.tcpPort
// }
// const tcpClient = net.createConnection(options, () =>{
//     console.log('Connection from' = tcpClient.remoteAddress + ":"+ tcpClient.remotePort);
// });
// tcpClient.setTimeout(1000);
// tcpClient.setEncoding('utf8');

// tcpClient.on('data',(e) => {
//     console.log('Pod Returned: '+ e);
// });

// tcpClient.on('end', () =>{
//     console.log('Pod Disconnected');
// });

// tcpClient.on('timeout', ()=>{
//     console.log('Lol pod timedout, you done goofed');
// });

// tcpClient.on('error', ()=>{
//     throw err;
// });

// return tcpClient;
// };

// var terminalOutput = getConnection();
// terminalOutput.write('Lol hey');
