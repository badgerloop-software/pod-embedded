const constants = require('../../constants');
const events = require('events');
const updater = new events.EventEmitter();
module.exports.updater = updater;
const storedData = require('../../database');
const MongoClient = require('mongodb');

const dbIP = constants.databaseAddr.ip;
const dbPort = constants.databaseAddr.port;

module.exports.updateData = function updateData(dataIn) {
    //Sort through the data and append the new values to their respective arrays in database.js
    let groups = Object.keys(dataIn);
    groups.forEach((i) => {
        let sensors = Object.keys(dataIn[i]);
        // console.log(i);
        sensors.forEach((sensor) => {
            input = dataIn[i][sensor];
            target = storedData[i][sensor];
            target.push(input);
        });
    });
    //Tell proto.js to render the data
    updater.emit('updateData');
}

function getMongoID() {
    id = String(String(new Date().getDate()) + String(new Date().getHours()) + String(new Date().getMinutes()));
    console.log(id);
    return String("run" + id);
}

module.exports.archiveData = function archiveData(id) {
    if (!id) {
        id = getMongoID();
    }
    MongoClient.connect(String("mongodb://" + constants.databaseAddr.ip + ":" + constants.databaseAddr.port), { useNewUrlParser: true }, function (err, db) {
        if (err) throw err;
        var dbo = db.db("BadgerloopRunData")
        dbo.createCollection(id, function (err, res) {
            if (err) throw err;
            console.log("Collection Created");
        })
        dbo.collection(id).insertOne(storedData, function (err, res) {
            if (err) throw err;
            db.close();
        })

    })
}

module.exports.createTestKeys = function createTestKeys(){
    console.log("creating test keys")
    for(var i=0; i<150; i++){
        storedData.test[i] = [1];
    }
}



