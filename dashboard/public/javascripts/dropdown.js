n/*
Author: Luke Houge
Purpose: Enable dynamic and searchable dropdowns
*/
function dropdown(num) {
  document.getElementById("myDropdown"+String(num)).classList.toggle("show");
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