const comms = require('./public/javascripts/client').recievedEmitter;
const client = require('./public/javascripts/client');
const di = require('./public/javascripts/DataInterfacing');
var constants = require('./constants');
var storedData = require('./database');
var d = document, db = document.body;
var archiveButton = d.getElementById("archiveButton");
var settingsSubmit = d.getElementById("podSettingsSubmit");


comms.on('heartbeat', function () {
    changeState("podConnect", true);
    console.log("Heartbeat Recieved");
});


//Data in recieved
comms.on('dataIn', function () {
    console.log("dataIn - Event Recieved");
    //Log it to be sure
    console.log(client.inData);
    //Tell the Data Interfacer to start sorting it ... On a second look I could scrap this middleman altogether, I'll look into this
    di.updateData(client.inData.data);
});

//Render command
di.updater.on('updateData', () => {
    let groups = Object.keys(storedData);
    groups.forEach((group) => {
        let sensors = Object.keys(storedData[group]);
        sensors.forEach((sensor) => {
            //Check to see if that particular sensor is being rendered at the time
            try {
                if(group !== "connections")
                updateData(group, sensor);
                
            } catch{
                //If not, alert the user and move on
                console.log("Unreconized Sensor- " + sensor +" -Skipping");
            }
            var now = Date.now();
            setAgeLabel(now - client.inData.age);
        });
    }
    )
});

function updateData(group, sensor) {
    // Get numbers
    let t = d.getElementById(String(sensor));
    let stored = storedData[group][sensor];
    //Set number
    if(stored[stored.length-1] == null){
        console.log(group +" " + sensor + " " + stored[stored.length-1]);
    }
    t.innerHTML = String(stored[stored.length - 1]);

}


 function setAgeLabel(staleness){
    d.getElementById('ageDisplay').innerHTML = String(staleness+"ms");
}

archiveButton.addEventListener("click", function() {
    di.archiveData();
    console.log("archiving data");
});

settingsSubmit.addEventListener("click", () => {
    constants.serverAddr.ip = d.getElementById("podIP").value;
    constants.serverAddr.port = Number(d.getElementById("podPort").value);
    constants.databaseAddr.ip = d.getElementById("databaseIP").value;
    constants.databaseAddr.port = Number(d.getElementById("databasePort").value);
    constants.scanningRate = Number(d.getElementById("scanningRate").value);
    d.getElementById("formFeedback").innerHTML = "Settings Applied";
});

function fillConstants() {
    d.getElementById("formFeedback").innerHTML = "";
    d.getElementById("podIP").value = String(constants.serverAddr.ip);
    d.getElementById("podPort").value = constants.serverAddr.port;
    d.getElementById("databaseIP").value = constants.databaseAddr.ip;
    d.getElementById("databasePort").value = constants.databaseAddr.port
    d.getElementById("scanningRate").value = constants.scanningRate;
}