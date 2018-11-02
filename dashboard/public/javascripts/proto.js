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

comms.on('dataIn', function () {
    console.log("dataIn - Event Recieved");
    console.log(client.inData);
    di.updateData(client.inData.data);
});

di.updater.on('updateData', () => {
    // var startDate = new Date();
    // var startTime = startDate.getMilliseconds();
    let groups = Object.keys(storedData);
    groups.forEach((group) => {
        let sensors = Object.keys(storedData[group]);
        sensors.forEach((sensor) => {
            try {
                updateData(group, sensor);
                var now = Date.now();
                setAgeLabel(now - client.inData.age);

            } catch{
                console.log("Unreconized Sensor- " + sensor) +" -Skipping";
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
    let t = document.getElementById(String(sensor));
    let tMin = Number(document.getElementById(String(sensor + "Min")).innerHTML);
    let tMax = Number(document.getElementById(String(sensor + "Max")).innerHTML);
    let stored = storedData[group][sensor];
    document.getElementById(String(sensor)).innerHTML = String(stored[stored.length - 1]);
    if (Number(t.innerHTML) > tMin && Number(t.innerHTML) < tMax) {
        setSensorGreen(sensor);
    } else
        setSensorRed(sensor);
}


 function setAgeLabel(staleness){
    d.getElementById('age').innerHTML = String(staleness);
}