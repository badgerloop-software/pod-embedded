//dropdown code
function openDropdown(num) {
  console.log("opening dropdown" + String(num));
  document.getElementById("myDropdown" + String(num)).classList.remove("hide");
  document.getElementById("myDropdown" + String(num)).classList.add("show");
  document.getElementById("dropdownInput" + String(num)).focus();
  }

function closeDropdown(num){
  console.log("closing dropdown" + String(num));
  document.getElementById("myDropdown" + String(num)).classList.remove("show");
  document.getElementById("myDropdown" + String(num)).classList.add("hide");
}
function closeAllDropdowns(){
  for(let i=1; i<4; i++){
    closeDropdown(i);
  }
}

  function filterFunction() {
    var input, filter, a, i;
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
  window.addEventListener("click", function(event) {
    targetNode = String(event.target.tagName);
    console.log(targetNode)
    if(targetNode !== "A"){
      if(targetNode !== "BUTTON"){
        closeAllDropdowns();
      }
    }
  });