//dynamically styles cells and table based on values in range or not

var tableIDs = ["motion", "braking", "battery_pack", "motor"]; //arrays for loop to iterate through
var divIDs = ["motion_div", "braking_div", "battery_pack_div", "motor_div"];
var statusIDs = ["motion_status", "braking_status", "battery_status", "motor_status"];

setInterval(function(){
  var w=0;
  for (var i = 0; i < 3; i++) {
    var table = document.getElementById(tableIDs[i]); //creates table array
      for (var r = 1, n = table.rows.length; r < n; r++) { //iterates through rows in given table
        var x = parseInt(table.rows[r].cells[1].innerHTML); //sets the min value to x
        var y = parseInt(table.rows[r].cells[2].innerHTML); //sets the value to y
        if (y<x) { //checks if too low
          table.rows[r].cells[2].style.backgroundColor = '#FC6962'; //makes red
          w++; //adds to w, signifying that there is an error present in the table
        }
        else {
          table.rows[r].cells[2].style.backgroundColor = '#fff'; //else sets to white background
        }
      }
    if (w != 0) { //if there was an error in any row during one run of the for loop, meaning w is not 0 as it was created as, then change the class of the div that tavble is in to 'error', which will make the border color red
      document.getElementById(divIDs[i]).className = "error";
      w=0;
    }
    else { //if there was not an error during the for loop in any row, then keep the class of the div as 'ok'
      document.getElementById(divIDs[i]).className = "ok"; 
      w=0;
    }
    //dummy function for status, 2-10= connected, 1= disconected
    var c =2;
    if (c > 1) {
      document.getElementById(statusIDs[i]).className = "connected"; 
    }
    if (c == 1) {
      document.getElementById(statusIDs[i]).className = "disconnected"; 
    }
  }
}, 100);