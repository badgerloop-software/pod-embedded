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
  
  //random generator, and highlightng (need to fix div outline highlighting to be more effective)
  setInterval(function(){
    var i, v, w;
    for (i=0; i< 100; i++) {
      v= Math.floor((Math.random()*100)+1);
      document.getElementsByClassName("rand")[i].innerHTML = v;
      w = 0
      if (v < 10) {
        document.getElementsByClassName("rand")[i].bgColor = '#FC6962';
        w++;
        if (i < 6) {
          document.getElementById("motion").className = "error";
        }
        else if (i > 6) {
          document.getElementById("braking").className = "error";
        }
      }
      else if (v > 90) {
        document.getElementsByClassName("rand")[i].bgColor = '#C8EEC4';
        if (i < 6) {
          document.getElementById("motion").className = "ok";
        }
        else if (i > 6) {
          document.getElementById("braking").className = "ok";
        }
      }
      else {
        document.getElementsByClassName("rand")[i].bgColor = '#fff';
      }
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



