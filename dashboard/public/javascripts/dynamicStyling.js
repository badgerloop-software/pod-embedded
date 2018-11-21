//random generator for motion
setInterval(function(){
  var w=0;
  for (i=1; i< 6; i++){ //for loop that goes through each row of the table, 'i' being used to represent row (should probably change to r)
    var x = document.getElementById("motion").rows[i].cells; //gets the cells in the row 'i' and sets to x
    var y;
    //sets a variable y to the value that is currently in the 3rd colunn (the value column)
    if (y<2) { //if that obtained value is less than 2 (arbitrary number for now), style red and add to counter w which will be used to determine if any row had an error
      x[2].style.backgroundColor = '#FC6962';
      w++;
    }
    else if (y>90) { //if that obtained value is greater than 90 (arbitrary value for now), style green
      x[2].style.backgroundColor = '#C8EEC4';
    }
    else { //if it is in between remain, or change back, to white background
      x[2].style.backgroundColor = '#fff';
    }
  }
  if (w != 0) { //if there was an error in any row during one run of the for loop, meaning w is not 0 as it was created as, then change the class of the div that tavble is in to 'error', which will make the border color red
    document.getElementById("motion_div").className = "error";
    w=0;
  }
  else { //if there was not an error during the for loop in any row, then keep the class of the div as 'ok'
    document.getElementById("motion_div").className = "ok"; 
    w=0;
  }
  //dummy function for status, 2-10= connected, 1= disconected
  var c;
  if (c > 1) {
    document.getElementById("motion_status").className = "connected"; 
  }
  if (c == 1) {
    document.getElementById("motion_status").className = "disconnected"; 
  }
}, 300);

//random generator for braking and pressures
setInterval(function(){
  var w=0;
  for (i=1; i< 9; i++){
    var x = document.getElementById("braking").rows[i].cells;
    var y;
    if (y<2) {
      x[2].style.backgroundColor = '#FC6962';
      w++;
    }
    else if (y>90) {
      x[2].style.backgroundColor = '#C8EEC4';
    }
    else {
      x[2].style.backgroundColor = '#fff';
    }
  }
  if (w != 0) {
    document.getElementById("braking_div").className = "error";
    w=0;
  }
  else {
    document.getElementById("braking_div").className = "ok"; 
    w=0;
  }
  //dummy function for status, 2-10= connected, 1= disconected
  var c;
  if (c > 1) {
    document.getElementById("breaking_status").className = "connected"; 
  }
  if (c == 1) {
    document.getElementById("breaking_status").className = "disconnected"; 
  }
}, 300);

//random generator for battery
setInterval(function(){
  var w=0;
  for (i=1; i< 9; i++){
    var x = document.getElementById("battery_pack").rows[i].cells;
    var y;
    if (y<2) {
      x[2].style.backgroundColor = '#FC6962';
      w++;
    }
    else if (y>90) {
      x[2].style.backgroundColor = '#C8EEC4';
    }
    else {
      x[2].style.backgroundColor = '#fff';
    }
  }
  if (w != 0) {
    document.getElementById("battery_pack_div").className = "error";
    w=0;
  }
  else {
    document.getElementById("battery_pack_div").className = "ok"; 
    w=0;
  }
  //dummy function for status, 2-10= connected, 1= disconected
  var c;
  if (c > 1) {
    document.getElementById("battery_status").className = "connected"; 
  }
  if (c == 1) {
    document.getElementById("battery_status").className = "disconnected"; 
  }
}, 300);

//random generator for motor
setInterval(function(){
  var w=0;
  for (i=1; i< 5; i++){
    var x = document.getElementById("motor").rows[i].cells;
    var y;
    if (y<2) {
      x[2].style.backgroundColor = '#FC6962';
      w++;
    }
    else if (y>90) {
      x[2].style.backgroundColor = '#C8EEC4';
    }
    else {
      x[2].style.backgroundColor = '#fff';
    }
  }
  if (w != 0) {
    document.getElementById("motor_div").className = "error";
    w=0;
  }
  else {
    document.getElementById("motor_div").className = "ok"; 
    w=0;
  }
  //dummy function for status, 2-10= connected, 1= disconected
  var c;
  if (c == 1) {
    document.getElementById("motor_status").className = "connected"; 
  }
  if (c >1) {
    document.getElementById("motor_status").className = "disconnected"; 
  }
}, 300);