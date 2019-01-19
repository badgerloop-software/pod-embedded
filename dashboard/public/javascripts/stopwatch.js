var Stopwatch = function() {
  var offset, interval, clock;
  // initialize
  reset();

  function start() {
      offset = Date.now();
      interval = setInterval(update, 1);

  }

  function stop() {
    if (interval) {
      clearInterval(interval);
      interval = null;
    }
  }

  function reset() {
    clock = 0;
  }

  function update() {
    clock += delta();
  }

  function delta() {
    var now = Date.now(),
      d = now - offset;

    offset = now;
    return d;
  }

  // public API
  this.start = start;
  this.stop = stop;
  this.reset = reset;
  this.currentTime = clock;
};

module.exports.stopwatch = Stopwatch;

//Lol I didn't write this
//https://stackoverflow.com/questions/20318822/how-to-create-a-stopwatch-using-javascript
