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

//random generator for motion
setInterval(function(){
  var w=0;
  for (i=1; i< 6; i++){
    var x = document.getElementById("motion").rows[i].cells;
    var y = Math.floor((Math.random()*100)+1) //change to data input
    x[2].innerHTML =  y;
    if (y<10) {
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
  //dummy function for status, 1= connected, 2= disconected
  var c = Math.floor((Math.random()*2)+1)
  if (c == 1) {
    document.getElementById("motion_status").className = "connected"; 
  }
  if (c == 2) {
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
    if (y<10) {
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
  //dummy function for status, 1= connected, 2= disconected
  var c = Math.floor((Math.random()*2)+1)
  if (c == 1) {
    document.getElementById("breaking_status").className = "connected"; 
  }
  if (c == 2) {
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
    if (y<10) {
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
  //dummy function for status, 1= connected, 2= disconected
  var c = Math.floor((Math.random()*2)+1)
  if (c == 1) {
    document.getElementById("battery_status").className = "connected"; 
  }
  if (c == 2) {
    document.getElementById("battery_status").className = "disconnected"; 
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

//clones table entry to table in header (currently IP)
function cloneRow1() {
  var row = document.getElementById("primarytank"); // find row to copy
  var table = document.getElementById("tableToModify"); // find table to append to
  var clone = row.cloneNode(true); // copy children too
  clone.id = "newID"; // change id or other attributes/contents
  table.appendChild(clone); // add new row to end of table
}
