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

function initGrid() {
    grid = d.querySelector('.grid');
    pckry = new Packery(grid, {
        itemSelector: '.grid-sizer'
    });

    pckry.getItemElements().forEach(function (itemElem) {
        var draggie = new Draggabilly(itemElem);
        pckry.bindDraggabillyEvents(draggie);
    });

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

function layoutGrid(){
    pckry.layout();
}
module.exports.layoutGrid = layoutGrid();

comms.on('heartbeat', function () {
    changeState("podConnect", true);
    console.log("Event Recieved");
});

comms.on('dataIn', function () {
    console.log("dataIn - Event Recieved")
    di.updateData(client.inData);
});

di.updater.on('updateData', () => {
    let group = Object.keys(storedData);
    group.forEach((group) => {
        let sensors = Object.keys(storedData[group]);
        if (group !== "connections") {
            sensors.forEach((i) => {
                updateData(group, i);
            });
        }
    })
});



function updateData(group, sensor) {
    let stored = storedData[group][sensor];
    document.getElementById(String(sensor)).innerHTML = String(stored[stored.length - 1]);
    console.log(document.getElementById(String(sensor)).innerHTML);
}


