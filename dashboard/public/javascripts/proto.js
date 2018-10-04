const comms = require('./public/javascripts/client').recievedEmitter;
var client = require('./public/javascripts/client'); 
const di = require('./public/javascripts/DataInterfacing');
var storedData = require('./database');
var d = document, db = document.body;
window.$ = window.jQuery = require('jquery');
var Packery = require('packery');
var Draggabilly = require('draggabilly');


window.onload = function () {
  initGrid();
}

function initGrid(){
    var grid = d.querySelector('.grid');
    var pckry = new Packery(grid, {
        itemSelector: '.grid-sizer'
    });

    pckry.getItemElements().forEach(function(itemElem){
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

comms.on('heartbeat', function () {
    changeState("podConnect", true);
    console.log("Event Recieved");
});

comms.on('dataIn',function(){
    console.log("dataIn - Event Recieved")
    di.updateData(client.inData);
});

di.updater.on('updateData', () =>{

})

