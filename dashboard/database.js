module.exports = {
    connections: {
        pod: false,
        NAP: false,
        BMS: false,
        RMS: false
    },
    braking: {
        solenoids: [],
        brakePadTemps: [],
        brakeGood: []
    },
    navigation : {
        imu: [],
        encoder: [],
        retro: [] 
    },
    pressureVessel: {
        pressure: [],
        airQuality: [],
        paranormal: []
    }
}