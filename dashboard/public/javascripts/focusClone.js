/*
Author: Luke Houge
Purpose: Clones table entry to header
*/
var x = 1;
function clone(id) {
  if (x == 1) {
    setInterval(function() {
      var value = document.getElementById(id).innerHTML;
      document.getElementById("header_value_1").innerHTML = value;
      var name = id.replace(/([a-z\xE0-\xFF])([A-Z\xC0\xDF])/g, "$1 $2");
      document.getElementById("header_label_1").innerHTML = name;
    }, 300);
    x++;
  } else if (x == 2) {
    setInterval(function() {
      var value = document.getElementById(id).innerHTML;
      document.getElementById("header_value_2").innerHTML = value;
      var name = id.replace(/([a-z\xE0-\xFF])([A-Z\xC0\xDF])/g, "$1 $2");
      document.getElementById("header_label_2").innerHTML = name;
    }, 300);
    x++;
  } else if (x == 3) {
    setInterval(function() {
      var value = document.getElementById(id).innerHTML;
      document.getElementById("header_value_3").innerHTML = value;
      var name = id.replace(/([a-z\xE0-\xFF])([A-Z\xC0\xDF])/g, "$1 $2");
      document.getElementById("header_label_3").innerHTML = name;
    }, 300);
    x++;
  } else if (x == 4) {
    setInterval(function() {
      var value = document.getElementById(id).innerHTML;
      document.getElementById("header_value_4").innerHTML = value;
      var name = id.replace(/([a-z\xE0-\xFF])([A-Z\xC0\xDF])/g, "$1 $2");
      document.getElementById("header_label_4").innerHTML = name;
    }, 300);
    x++;
  } else if (x > 4) {
    alert("Max of 4 values reached, please remove one and try again");
  }
}

//clear for focus div
//FYI This function didn't work before I condensed it -E
function clear() {
  for(i=1; i<5; i++){
    document.getElementById("header_value_"+String(i)).innerHTML = "";
    document.getElementById("header_label_"+String(i)).innerHTML = "Value "+String(i);  
  }
}