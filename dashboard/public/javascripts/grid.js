var d = document, db = document.body;
var $grid;
function initGrid(){
 $grid = $('.grid').packery({
itemSelector: '.grid-item',
// columnWidth helps with drop positioning
columnWidth: 100
});

// make all grid-items draggable
$grid.find('.grid-item').each( function( i, gridItem ) {
var draggie = new Draggabilly( gridItem );
// bind drag events to Packery
$grid.packery( 'bindDraggabillyEvents', draggie );
});

}
function changeSize(t){
  if(t.classList.contains('grid-item--width2')){
    t.setAttribute('class', 'grid-item')
  }else{
    t.setAttribute('class', "grid-item grid-item--width2")
}
$grid.packery('layout');
}
function addGridItem(size){
  var targetClass;
  var temp = document.createElement("div");
  var button = d.createElement("div");
  button.setAttribute("class", "dropdown")
  button.innerHTML ='<button class="btn btn-secondary dropdown-toggle" type="button" id="dropdownMenuButton" data-toggle="dropdown" aria-haspopup="true" aria-expanded="true"></button>' +
     ' <div class="dropdown-menu" aria-labelledby="dropdownMenuButton">' +
      '<button type="button" class="btn btn-secondary" data-toggle="modal" data-target="#exampleModalCenter">Edit Data Points</button>' +
         ' <a id="sizeButton" class="dropdown-item" href="#">Edit Size</a>' +
         ' <a class="dropdown-item" href="#">Delete Tile</a>' +
      '</div>';
  /*
  0 = Small
  1 = Width 2
  2 = Height 2
  3 = Large
  */
 switch(size){
   case 0:
   targetClass = "";
      console.log("In SC")
      break;
      case 1:
      targetClass = "grid-item--width2";
      break;
  case 2:
  targetClass = "grid-item--height2";
      break;
      case 3:
      targetClass = "grid-item--large";
      break;
  default:
      targetClass = "";
    }
  temp.setAttribute("class", "grid-item"+" "+targetClass);
  temp.appendChild(button);
  var $item = $(temp);
  d.getElementById("mainGrid").appendChild(temp);
  $grid.append($item).packery('appended', $item)
  $item.each(makeEachDraggable)
}
//Tbh not sure what this does, I got it from a demo and I need it work 
function makeEachDraggable(i, itemElem){
  var draggie = new Draggabilly(itemElem);
  $grid.packery('bindDraggabillyEvents', draggie);
}


d.addEventListener('click', function(e){
  e = e || window.event;
  var target = e.target || e.srcElement;
  var pt = target.parentNode.parentNode.parentNode;
  if(pt.classList.contains("grid-item")){
    if(target.innerHTML == "Edit Size"){
      changeSize(target.parentNode.parentNode.parentNode);
    }
    if(target.innerHTML == "Delete Tile"){
      $grid.packery('remove', pt).packery('layout');
    }
  }else{
    console.log(pt);
  }
});