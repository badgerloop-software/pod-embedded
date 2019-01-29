/*
Author: Luke Houge
Purpose: Enable dynamic and searchable dropdowns
*/
function dropdown(num) {
  if (num==1){
    document.getElementById("myDropdown1").classList.toggle("show");
  }
  else if (num ==2){
    document.getElementById("myDropdown2").classList.toggle("show");
  }
  else if (num ==3){
    document.getElementById("myDropdown3").classList.toggle("show");
  }
}

function filterFunction(a) {
  var inputnum;
  var dropnum;
  if (a==1){
    inputnum = "dropdownInput1";
    dropnum = "myDropdown1";
  }
  else if (a==2){
    inputnum = "dropdownInput2";
    dropnum = "myDropdown2";
  }
  else if (a==3){
    inputnum = "dropdownInput3";
    dropnum = "myDropdown3";
  }
  var input, filter, ul, li, a, i;
  input = document.getElementById(inputnum);
  filter = input.value.toUpperCase();
  div = document.getElementById(dropnum);
  a = div.getElementsByTagName("a");
  for (i = 0; i < a.length; i++) {
      if (a[i].innerHTML.toUpperCase().indexOf(filter) > -1) {
          a[i].style.display = "";
      } else {
          a[i].style.display = "none";
      }
  }
}