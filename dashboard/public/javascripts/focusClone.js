//clones table entry to header
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
 }
