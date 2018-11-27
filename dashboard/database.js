module.exports = {
    connections: {
        pod: false,
        NAP: false,
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
    motion : {
        stoppingDistance: [],
        position: [],
        retro: [],
        velocity: [],
        acceleration: []
    },
    battery :{
        packVoltage: [],
        packCurrent: [],
        packSOC: [],
        packAH: [],
        cellMaxVoltage: [],
        cellMinVoltage: [],
        highTemp: [],
        lowTemp: []
    }
}