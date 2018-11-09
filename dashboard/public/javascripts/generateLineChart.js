chartState = [0, 0, 0, 0];

function generateLineChart(id, tdID, title, chartType) {
    var data = require("./public/javascripts/getData");
    
    var xmax = 1000; //maximum x-axis range in seconds. Should be the total runtime
    var sampleRate = 300; // sample rate in ms per sample

    var layout = {
    xaxis: {autorange: true},
    title: title,
    width: 450,
    height: 400,
    margin: {
        l: 40,
        r: 40,
        b: 40,
        t: 40,
      }
    };
    
    if (id == 'lineChartOne') {
        if (chartState[0] == 0) {        // checks if chart one is empty
            newChart();
            chartState[0] = 1;
        }
        else if (chartState[1] == 0) {  // checks if chart one has second trace
            
        }
    }

    function newChart() {
    chartState[0] = 1;
    Plotly.newPlot(id, [{
        y:[data.getData(tdID)],
        type: 'scatter',
        mode: 'lines',
        line: {color:'green'}
    }], layout);

    setInterval(function(){
        var currentData = data.getData(tdID);
        Plotly.extendTraces(id,{ y:[[currentData]]}, [0]);
    }, sampleRate);
    }

    function addTrace() {

    }

}

    