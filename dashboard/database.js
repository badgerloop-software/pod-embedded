module.exports = {
    connections: {
        pod: false,
        NAP: false,
        BMS: false,
        RMS: false
    },
    braking: {
        solenoids: [0],
        brakePadTemps: [0],
        brakeGood: [255]
    },
    navigation : {
        IMU: [123],
        encoder: [321],
        retro: [132] 
    },
    pressureVessel: {
        pressure: [13],
        airQuality: [23],
        paranormal: [0]
    }
}