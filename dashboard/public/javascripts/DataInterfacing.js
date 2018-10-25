const events = require('events');
const updater = new events.EventEmitter();
module.exports.updater = updater;
const storedData = require('../../database');
module.exports.updateData = function updateData(dataIn){
    let keys = Object.keys(dataIn);
    keys.forEach((i) => {
        let sensors = Object.keys(dataIn[i]);
        //console.log(i);
        sensors.forEach((sensor) =>{
            input = dataIn[data][i][sensor];
            target = storedData[i][sensor];
            target.push(input);
            console.log(target);
        });
    });
    updater.emit('updateData');
}