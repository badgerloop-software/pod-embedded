module.exports = {
    connections: {
        pod: false,
        NAP: false,
        BMS: false,
        RMS: false
    },
    braking: {
        primaryTank: [],
        primaryLine: [],
        primaryActuation: [],
        secondaryTank: [],
        secondaryLine: [],
        secondaryActuation: [],
        pressureVesselPressure: [],
        currentPressure: []
    },
    navigation : {
        imu: [],
        position: [],
        retro: [],
        velocity: [],
        acceleration: []
    },
    pressureVessel: {
        pressure: [],
        airQuality: [],
        paranormal: []
    }
}