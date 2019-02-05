/*
Author: Luke Houge, Eric Udlis
Purpose: Enable dynamic and searchable dropdowns
*/

// determines which dropdown is being triggered
function dropdown(num) {
  document.getElementById("myDropdown"+String(num)).classList.toggle("show");
}

//search filter function for  dropdowns
function filterFunction(a) {
  //determines which dropdown (1,2, or 3) is being called
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

  //filter function
  var input, filter, a, i;
  input = document.getElementById(inputnum);
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