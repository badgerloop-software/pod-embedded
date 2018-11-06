const constants = require('../../constants');
const events = require('events');
const updater = new events.EventEmitter();
module.exports.updater = updater;
const storedData = require('../../database');
const MongoClient = require('mongodb');

const dbIP = constants.databaseAddr.ip;
const dbPort = constants.databaseAddr.port;


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

module.exports.getMongoUrl = function getMongoUrl(id){
    //Specify id if needed, if you do not specify one then one will be created
    if(!id){
        //Generated ids are datehourminute
    generatedId = String(String(new Date().getDate())+ String(new Date().getHours())+String(new Date().getMinutes()));
    console.log(generatedId);
    id = generatedId;
    }
    return String("mongodb://" + dbIP +":" + dbPort +"/" + id);
}

module.exports.archiveData = function archiveData(id){
    MongoClient.connect(this.getMongoUrl(id), function(err, db){
        if(err) throw err;
        var dbo = db.db(String(id));
        dbo.createCollection("runData", function(err, res){
            if(err) throw err;
            console.log("Collection Created");
        })
        dbo.collection("runData").insertMany(dataObj, function(err, res){
            if(err) throw err;
            db.close();
        })

    })
}

