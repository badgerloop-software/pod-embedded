const comms = require('./public/javascripts/client').recievedEmitter;
var client = require('./public/javascripts/client');
const di = require('./public/javascripts/DataInterfacing');
var storedData = require('./database');
var d = document, db = document.body;
window.$ = window.jQuery = require('jquery');
var Packery = require('packery');
var Draggabilly = require('draggabilly');
var grid;
var pckry;


window.onload = function () {
    initGrid();
}

function layoutGrid() {
    pckry.layout();
}
function initGrid() {
    grid = d.querySelector('.grid');
    pckry = new Packery(grid, {
        itemSelector: '.grid-sizer'
    });

    pckry.getItemElements().forEach(function (itemElem) {
        var draggie = new Draggabilly(itemElem);
        pckry.bindDraggabillyEvents(draggie);
    });

    layoutGrid();

}
function setSensorGreen(sensor) {
    let t = document.getElementById(String(sensor));
    t.classList.remove('table-danger');
    t.classList.add('table-success');
}
function setSensorRed(sensor) {
    let t = document.getElementById(String(sensor));
    t.classList.remove('table-success');
    t.classList.add('table-danger');
}
function setSensorDefault(sensor) {
    let t = document.getElementById(String(sensor));
    t.classList.remove('table-success');
    t.classList.remove('table-danger');
}



function changeState(id, state) { //DOM ID, boolean
    t = d.getElementById(id);
    if (state) {
        t.setAttribute("class", "alert alert-success");
        t.innerHTML = "Connected"
    } else {
        t.setAttribute("class", "alert alert-danger");
        t.innerHTML = "Disconnected"
    }
}



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
    // var startDate = new Date();
    // var startTime = startDate.getMilliseconds();
    let groups = Object.keys(storedData);
    groups.forEach((group) => {
        let sensors = Object.keys(storedData[group]);
        sensors.forEach((sensor) => {
            //Check to see if that particular sensors is being rendered at the time
            try {
                updateData(group, sensor);
                var now = Date.now();
                setAgeLabel(now - client.inData.age);

            } catch{
                //If not, alert the user and move on
                console.log("Unreconized Sensor- " + sensor +" -Skipping");
            }
        });
    }
    )
    // var endDate = new Date();
    // var endTime = endDate.getMilliseconds();
    // var timeElasped = endTime - startTime;
    // console.log(timeElasped + " milliseconds elasped");
});

function updateData(group, sensor) {
    // Get numbers
    let t = d.getElementById(String(sensor));
    let tMin = Number(d.getElementById(String(sensor + "Min")).innerHTML);
    let tMax = Number(d.getElementById(String(sensor + "Max")).innerHTML);
    let stored = storedData[group][sensor];
    //Set numbers
    d.getElementById(String(sensor)).innerHTML = String(stored[stored.length - 1]);
    //Add warning if necessary
    if (Number(t.innerHTML) > tMin && Number(t.innerHTML) < tMax) {
        setSensorGreen(sensor);
    } else
        setSensorRed(sensor);
}


 function setAgeLabel(staleness){
    d.getElementById('age').innerHTML = String(staleness);
}