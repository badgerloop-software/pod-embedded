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
  var input, filter, ul, li, a, i;
  input = document.getElementById("dropdownInput"+String(a));
  filter = input.value.toUpperCase();
  div = document.getElementById("myDropdown"+String(a));
  a = div.getElementsByTagName("a");
  for (i = 0; i < a.length; i++) {
      if (a[i].innerHTML.toUpperCase().indexOf(filter) > -1) {
          a[i].style.display = "";
      } else {
          a[i].style.display = "none";
      }
  }
}