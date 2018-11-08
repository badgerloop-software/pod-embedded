//search for motion table
function searchmotion() {
  var input, filter, table, tr, td, i;
  input = document.getElementById("motioninput");
  filter = input.value.toUpperCase();
  table = document.getElementById("motion");
  tr = table.getElementsByTagName("tr");
  for (i = 0; i < tr.length; i++) {
    td = tr[i].getElementsByTagName("td")[0];
    if (td) {
      if (td.innerHTML.toUpperCase().indexOf(filter) > -1) {
        tr[i].style.display = "";
      } else {
        tr[i].style.display = "none";
      }
    }
  }
}

//search for braking table
function searchbreak() {
  var input, filter, table, tr, td, i;
  input = document.getElementById("brakinginput");
  filter = input.value.toUpperCase();
  table = document.getElementById("braking");
  tr = table.getElementsByTagName("tr");
  for (i = 0; i < tr.length; i++) {
    td = tr[i].getElementsByTagName("td")[0];
    if (td) {
      if (td.innerHTML.toUpperCase().indexOf(filter) > -1) {
        tr[i].style.display = "";
      } else {
        tr[i].style.display = "none";
      }
    }
  }
}

//search for battery table
function search_battery_pack() {
  var input, filter, table, tr, td, i;
  input = document.getElementById("battery_pack_input");
  filter = input.value.toUpperCase();
  table = document.getElementById("battery_pack");
  tr = table.getElementsByTagName("tr");
  for (i = 0; i < tr.length; i++) {
    td = tr[i].getElementsByTagName("td")[0];
    if (td) {
      if (td.innerHTML.toUpperCase().indexOf(filter) > -1) {
        tr[i].style.display = "";
      } else {
        tr[i].style.display = "none";
      }
    }
  }
}

//search for motor table
function searchmotor() {
  var input, filter, table, tr, td, i;
  input = document.getElementById("motorinput");
  filter = input.value.toUpperCase();
  table = document.getElementById("motor");
  tr = table.getElementsByTagName("tr");
  for (i = 0; i < tr.length; i++) {
    td = tr[i].getElementsByTagName("td")[0];
    if (td) {
      if (td.innerHTML.toUpperCase().indexOf(filter) > -1) {
        tr[i].style.display = "";
      } else {
        tr[i].style.display = "none";
      }
    }
  }
}

//random generator for motion
setInterval(function(){
  var w=0;
  for (i=1; i< 6; i++){
    var x = document.getElementById("motion").rows[i].cells;
    var y = Math.floor((Math.random()*100)+1) //change to data input
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
  var c = Math.floor((Math.random()*10)+1)
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
    var y = Math.floor((Math.random()*100)+1) //change to data input
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
  var c = Math.floor((Math.random()*10)+1)
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
  for (i=1; i< 5; i++){
    var x = document.getElementById("battery_pack").rows[i].cells;
    var y = Math.floor((Math.random()*100)+1) //change to data input
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
  var c = Math.floor((Math.random()*10)+1)
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
  for (i=1; i< 6; i++){
    var x = document.getElementById("motor").rows[i].cells;
    var y = Math.floor((Math.random()*100)+1) //change to data input
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
  var c = Math.floor((Math.random()*10)+1)
  if (c == 1) {
    document.getElementById("motor_status").className = "connected"; 
  }
  if (c >1) {
    document.getElementById("motor_status").className = "disconnected"; 
  }
}, 300);

//dropdown code
function dropdown() {
  document.getElementById("myDropdown").classList.toggle("show");
}
function filterFunction() {
  var input, filter, ul, li, a, i;
  input = document.getElementById("myInput");
  filter = input.value.toUpperCase();
  div = document.getElementById("myDropdown");
  a = div.getElementsByTagName("a");
  for (i = 0; i < a.length; i++) {
      if (a[i].innerHTML.toUpperCase().indexOf(filter) > -1) {
          a[i].style.display = "";
      } else {
          a[i].style.display = "none";
      }
  }
}

//clones table entry to header (currently IP)
var x=1
function clone(id) {
  if (x==1){
    setInterval(function(){
      var value = document.getElementById(id).innerHTML;
      document.getElementById('header_value_1').innerHTML = value;
      document.getElementById('header_label_1').innerHTML = id;
    }, 300);
    x++;
  }
  else if (x==2){
    setInterval(function(){
      var value = document.getElementById(id).innerHTML;
      document.getElementById('header_value_2').innerHTML = value;
      document.getElementById('header_label_2').innerHTML = id;
    }, 300);
    x++;
  }
  else if (x==3){
    setInterval(function(){
      var value = document.getElementById(id).innerHTML;
      document.getElementById('header_value_3').innerHTML = value;
      document.getElementById('header_label_3').innerHTML = id;
    }, 300);
    x++;
  }
  else if (x==4){
    setInterval(function(){
      var value = document.getElementById(id).innerHTML;
      document.getElementById('header_value_4').innerHTML = value;
      document.getElementById('header_label_4').innerHTML = id;
    }, 300);
    x++;
  }
  else if (x > 4){
    alert("Max of 4 values reached, please remove one and try again");
  }
}

//clear for focus div
function clear() {
  document.getElementById('header_value_1').innerHTML = "";
  document.getElementById('header_label_1').innerHTML = "Value 1";
  document.getElementById('header_value_2').innerHTML = "";
  document.getElementById('header_label_2').innerHTML = "Value 2";
  document.getElementById('header_value_3').innerHTML = "";
  document.getElementById('header_label_3').innerHTML = "Value 3";
  document.getElementById('header_value_4').innerHTML = "";
  document.getElementById('header_label_4').innerHTML = "Value 4";
  throw "";
}
