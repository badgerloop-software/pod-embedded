const events = require('events');
const updater = new events.EventEmitter();
module.exports.updater = updater;
const storedData = require('../../database');

module.exports.updateData = function updateData(dataIn){
    //Sort through the data and append the new values to their respective arrays in database.js
    let groups = Object.keys(dataIn);
    groups.forEach((i) => {
        let sensors = Object.keys(dataIn[i]);
        // console.log(i);
        sensors.forEach((sensor) =>{
            input = dataIn[i][sensor];
            target = storedData[i][sensor];
            target.push(input);
        });
    });
    //Tell proto.js to render the data
    updater.emit('updateData');
}

//I plan to have more stuff here