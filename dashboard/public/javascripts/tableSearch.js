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
  
  //search for motor table
  function searchmotor() {
    var input, filter, table, tr, td, i;
    input = document.getElementById("motorinput");
    filter = input.value.toUpperCase();
    table = document.getElementById("motor");
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
  