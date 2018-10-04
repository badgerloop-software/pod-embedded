var solenoids,brakepadTemps,brakeGood;
var IMU,encoder,retro;
var pressure,airQuality,paranormal
module.exports =  {
    serverAddr: {
        ip : "127.0.0.1",
        port : "33333"
    },
    braking: [solenoids, brakepadTemps, brakeGood],
    navigation: [IMU, encoder, retro],
    pressureVessel: [pressure, airQuality, paranormal]
    
}