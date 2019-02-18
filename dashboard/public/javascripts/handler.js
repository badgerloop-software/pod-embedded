/*
Author: Eric Udlis
Purpose: Handle all updaters and interfacing between the frontend and backend
*/
const client = require("./public/javascripts/communication");
const di = require("./public/javascripts/DataInterfacing");
const comms = require("./public/javascripts/communication").recievedEmitter;
var constants = require("./constants");
var storedData = require("./database");
var d = document;
var archiveButton = d.getElementById("archiveButton");
var settingsSubmit = d.getElementById("podSettingsSubmit");
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
  //Tell the Data Interfacer to start sorting it
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
  if (elapsedTime > 0) {
    setAgeLabel(elapsedTime);
  }
});

// Update the Database and Render the latest entry
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

//Sets the latency counter
function setAgeLabel(staleness) {
  d.getElementById("ageDisplay").innerHTML = String(staleness + "ms");
}

//Handles the archive button click
archiveButton.addEventListener("click", function() {
  di.archiveData();
  console.log("archiving data");
});

//Handles power off button click
one.addEventListener("click", function() {
  console.log("powering off")
});

//Handles Idle/Ready button click
two.addEventListener("click", function() {
  console.log("idling")
});

//Handles Post Run/service low speed/Safe to Approach button click
three.addEventListener("click", function() {
  console.log("safe to approach")
});

//Handles pumpdown/ready for pumpdown button click
four.addEventListener("click", function() {
  console.log("pumpdown")
});

//Handles hyperloop/openair/external subtrack button click
five.addEventListener("click", function() {
  console.log("hyperloop")
});

//Handles pre/during/post run buttons
six.addEventListener("click", function() {
  console.log("run status")
});

//Handles primary brake on/off button click
seven.addEventListener("click", function() {
  console.log("primary brake status")
});
//Settings Form

//Submits Entries to File
settingsSubmit.addEventListener("click", () => {
  constants.serverAddr.ip = d.getElementById("podIP").value;
  constants.serverAddr.port = Number(d.getElementById("podPort").value);
  constants.databaseAddr.ip = d.getElementById("databaseIP").value;
  constants.databaseAddr.port = Number(d.getElementById("databasePort").value);
  constants.scanningRate = Number(d.getElementById("scanningRate").value);
  d.getElementById("formFeedback").innerHTML = "Settings Applied";
});

//Fills entries in text boxes
function fillConstants() {
  d.getElementById("formFeedback").innerHTML = "";
  d.getElementById("podIP").value = String(constants.serverAddr.ip);
  d.getElementById("podPort").value = constants.serverAddr.port;
  d.getElementById("databaseIP").value = constants.databaseAddr.ip;
  d.getElementById("databasePort").value = constants.databaseAddr.port;
  d.getElementById("scanningRate").value = constants.scanningRate;
}
