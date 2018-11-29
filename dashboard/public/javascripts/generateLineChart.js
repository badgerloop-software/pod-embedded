chartState = [0, 0, 0, 0];
chartTDID = ["", "", "", ""];
traceArray = [0];
chartData = [0, 0, 0, 0];
numTraces = 0
yData = []

function generateLineChart(id, tdID, title, chartType) {
    //var data = require("./public/javascripts/getData");
    
    var xmax = 100; //maximum x-axis range in seconds. Should be the total runtime
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
            chartTDID[0] = tdID;
            numTraces = 1;
            document.getElementById('test').innerHTML = yData;
            newChart();
            chartState[0] = 1;
            
        } 
        else if (chartState[1] == 0) {  // checks if chart one has second trace
            chartTDID[1] = tdID;
            traceArray = [0, 1]
            numTraces = 2;
            addTrace();
            chartState[1] = 1;
            //document.getElementById('test').innerHTML = chartData[0];
        }
    }

    function newChart() {
        getData();
        Plotly.newPlot(id, [{
            y: [chartData[0]],
            type: 'scatter',
            mode: 'lines',
            line: {color:'green'}
        }], layout);

        getDataAtInterval();
    }

    function addTrace() {
        Plotly.addTraces(id, [{
            y:[chartData[1]],
            type: 'scatter',
            mode: 'lines',
            line: {color:'blue'}
        }]);
        
        getDataAtInterval();
    }

    function getDataAtInterval() {
        setInterval(function(){
            getData();
            if (numTraces == 1) {
                yData = [[chartData[0]]]
            }
            else if (numTraces == 2) {
                yData = [[chartData[0]], [chartData[1]]]
            }
            Plotly.extendTraces(id, {y:yData}, traceArray);
        }, sampleRate);
    }
    
    function getData() {
        for (i=0; i<=(numTraces-1); i++) {
            var value = document.getElementById(chartTDID[i]);
            chartData[i] = value.innerHTML;
        }      
    }
}
