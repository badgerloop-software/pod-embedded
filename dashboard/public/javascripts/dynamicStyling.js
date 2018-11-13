//random generator for motion
setInterval(function(){
  var w=0;
  for (i=1; i< 6; i++){
    var x = document.getElementById("motion").rows[i].cells;
    var y;
    x[2].innerHTML =  y;
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
    document.getElementById("motion_div").className = "error";
    w=0;
  }
  else {
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
    x[2].innerHTML =  y;
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
    x[2].innerHTML =  y;
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
    x[2].innerHTML =  y;
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