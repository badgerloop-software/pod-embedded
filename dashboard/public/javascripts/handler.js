const comms = require("./public/javascripts/client").recievedEmitter;
var client = require("./public/javascripts/client");
const di = require("./public/javascripts/DataInterfacing");
const stopwatch = require("./public/javascripts/stopwatch").stopwatch;
var storedData = require("./database");
var d = document,
  db = document.body;
var archiveButton = d.getElementById("archiveButton");
var timeOld;

comms.on("heartbeat", function() {
  changeState("podConnect", true);
  console.log("Heartbeat Recieved");
});

//Data in recieved
comms.on("dataIn", function() {
  console.log("dataIn - Event Recieved");
  //Log it to be sure
  console.log(client.inData);
  //Tell the Data Interfacer to start sorting it ... On a second look I could scrap this middleman altogether, I'll look into this
  di.updateData(client.inData.data);
});

//Render command
di.updater.on("updateData", () => {
  var counter = new Date();
  var elapsedTime;
  var timeNew = counter.getMilliseconds();
  
  let groups = Object.keys(storedData);
  
  groups.forEach(group => {
    let sensors = Object.keys(storedData[group]);
    sensors.forEach(sensor => {
      //Check to see if that particular sensor is being rendered at the time
      try {
        if (group !== "connections") updateData(group, sensor);
      } catch {
        //If not, alert the user and move on
        console.log("Unreconized Sensor- " + sensor + " -Skipping");
      }
    });
  });
  
  //Lag Counter, when testing should be equal to DATA_SEND_RATE
  if (!timeOld) {
    elapsedTime = counter.getMilliseconds() - timeNew;
  } else {
    elapsedTime = timeNew - timeOld;
  }
  timeOld = timeNew;
  if(elapsedTime > 0){
      setAgeLabel(elapsedTime);
  }
});

function updateData(group, sensor) {
  // Get numbers
  let t = d.getElementById(String(sensor));
  let stored = storedData[group][sensor];
  //Set number
  if (stored[stored.length - 1] == null) {
    console.log(group + " " + sensor + " " + stored[stored.length - 1]);
  }
  t.innerHTML = String(stored[stored.length - 1]);
}

function setAgeLabel(staleness) {
  d.getElementById("ageDisplay").innerHTML = String(staleness + "ms");
}

archiveButton.addEventListener("click", function() {
  di.archiveData();
  console.log("archiving data");
});
