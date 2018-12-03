//random generator for motion
setInterval(function(){
  var w=0;
  var table = document.getElementById('motion'); //creates table array
    for (var r = 1, n = table.rows.length; r < n; r++) { //iterates through rows in given table
      var y = table.rows[r].cells[2].innerHTML; //sets the value to y
      var x = table.rows[r].cells[1].innerHTML; //sets the min value to x
      if (y<x) { //checks if too low
        table.rows[r].cells[2].style.backgroundColor = '#FC6962'; //makes red
        w++; //adds to w, signifying that there is an error present in the table
      }
      else {
        table.rows[r].cells[2].style.backgroundColor = '#fff'; //else sets to white background
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
  var c =2;
  if (c > 1) {
    document.getElementById("motion_status").className = "connected"; 
  }
  if (c == 1) {
    document.getElementById("motion_status").className = "disconnected"; 
  }
}, 100);

//random generator for braking and pressures
setInterval(function(){
  var w=0;
  var table = document.getElementById('braking');
    for (var r = 1, n = table.rows.length; r < n; r++) {
      var y = table.rows[r].cells[2].innerHTML;
      var x = table.rows[r].cells[1].innerHTML; //sets the min value to x
      if (y<x) { //checks if too low
        table.rows[r].cells[2].style.backgroundColor = '#FC6962';
        w++;
      }
      else {
        table.rows[r].cells[2].style.backgroundColor = '#fff';
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
  var c =2;
  if (c > 1) {
    document.getElementById("braking_status").className = "connected"; 
  }
  if (c == 1) {
    document.getElementById("braking_status").className = "disconnected"; 
  }
}, 100);

//random generator for battery
setInterval(function(){
  var w=0;
  var table = document.getElementById('battery_pack');
    for (var r = 1, n = table.rows.length; r < n; r++) {
      var y = table.rows[r].cells[2].innerHTML;
      var x = table.rows[r].cells[1].innerHTML; //sets the min value to x
      if (y<x) { //checks if too low
        table.rows[r].cells[2].style.backgroundColor = '#FC6962';
        w++;
      }
      else {
        table.rows[r].cells[2].style.backgroundColor = '#fff';
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
 var c =2;
 if (c > 1) {
   document.getElementById("battery_status").className = "connected"; 
 }
 if (c == 1) {
   document.getElementById("battery_status").className = "disconnected"; 
 }
}, 100);

//random generator for motor
setInterval(function(){
  var w=0;
  var table = document.getElementById('motor');
    for (var r = 1, n = table.rows.length; r < n; r++) {
      var y = table.rows[r].cells[2].innerHTML;
      var x = table.rows[r].cells[1].innerHTML; //sets the min value to x
      if (y<x) { //checks if too low
        table.rows[r].cells[2].style.backgroundColor = '#FC6962';
        w++;
      }
      else {
        table.rows[r].cells[2].style.backgroundColor = '#fff';
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
  var c =1;
  if (c > 1) {
    document.getElementById("motor_status").className = "connected"; 
  }
  if (c == 1) {
    document.getElementById("motor_status").className = "disconnected"; 
  }
}, 100);