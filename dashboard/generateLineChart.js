
var data = require("./getData");
var xmax = 1000; //maximum x-axis range in seconds. Should be the total runtime

var layout = {
    xaxis: {range: [0, xmax]},
  };

Plotly.newPlot('chart', [{
    y:[data.getData()],
    type: 'line'
}], layout);

var cnt = 0;
var sampleRate = 50; // sample rate in ms per sample
var colorArray = [];

setInterval(function(){
    var currentData = data.getData();
    Plotly.extendTraces('chart',{ y:[[currentData]]}, [0]);
}, sampleRate);

    