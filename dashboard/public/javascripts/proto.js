var d=document,db=document.body;
var f = false;
function initGrid(){
    $grid = $('.grid')
    $grid.packery({
   itemSelector: '.grid-sizer',
   // columnWidth helps with drop positioning
   columnWidth: $grid.find('.col-sm')[0],
   percentPosition: true
   });
   
   // make all grid-items draggable
   $grid.find('.grid-sizer').each( function( i, gridItem ) {
   var draggie = new Draggabilly( gridItem );
   // bind drag events to Packery
   $grid.packery( 'bindDraggabillyEvents', draggie );
   });
}

 function changeState(id,state){ //DOM ID, boolean
    t = d.getElementById(id);
    if(state){
        t.setAttribute("class", "alert alert-success");
        t.innerHTML = "Connected"
    }else{
        t.setAttribute("class", "alert alert-danger");
        t.innerHTML = "Disconnected"
    }
 }


  module.exports.podConnect = d.getElementById("connectID");
  module.exports.napConnect = d.getElementById("napConnect");
  module.exports.bmsConnect = d.getElementById("bmsConnect");
  module.exports.rmsConnect = d.getElementById("rmsConnect");
  module.exports.changeState = changeState;
  