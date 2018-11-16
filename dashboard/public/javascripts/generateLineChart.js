chartState = [0, 0, 0, 0];
chartTDID = ["", "", "", ""];
traceArray = [0];
chartData = [0, 0, 0, 0];
numTraces = 0;
yData = [];
xpos = 0;
chartFirstCreation = 0;
chartTitles = ["", "", "", ""]

function generateLineChart(id, tdID, title) {
    //var data = require("./public/javascripts/getData");
    
    var xmax = 100; //maximum x-axis range in seconds. Should be the total runtime
    var sampleRate = 300; // sample rate in ms per sample

    var layout = {
    xaxis: {range: [0, xmax]},
    title: "",
    width: 220,
    height: 220,
    margin: {
        l: 30,
        r: 20,
        b: 20,
        t: 30,
      }
    };

    if (id == 'lineChartOne') {
        if (chartState[0] == 0) {        // checks if chart one is empty
            chartTDID[0] = tdID;
            traceArray = [0];
            chartTitles[0] = title;
            layout['title'] = chartTitles[0];
            numTraces = 1;
            //document.getElementById("focus_add_button").innerHTML = chartTitles[0]
            newChart();
            chartFirstCreation = 1;
            chartState[0] = 1;
            
        } 
        else if (chartState[1] == 0) {  // checks if chart one has second trace
            chartTDID[1] = tdID;
            traceArray = [0, 1]
            numTraces = 2;
            chartTitles[1] = title
            layout['title'] = chartTitles[0]+ ', <br>' + chartTitles[1];
            addTrace();
            chartState[1] = 1;
        }
    }

    function newChart() {
        getData();
        Plotly.newPlot(id, [{
            x: [xpos],
            y: [chartData[0]],
            type: 'scatter',
            mode: 'lines',
            line: {color:'green'}
        }], layout);

        if (chartFirstCreation == 0) {
            getDataAtInterval();
        }
    }

    function addTrace() {
        Plotly.relayout(id, layout);
        Plotly.addTraces(id, [{
            x: [xpos],
            y:[chartData[1]],
            type: 'scatter',
            mode: 'lines',
            line: {color:'blue'}
        }]);
    }

    function getDataAtInterval() {
        setInterval(function(){
            xpos += 1;
            getData();
            if (numTraces == 1) {
                var update = {
                    x: [[xpos]],
                    y: [[chartData[0]]]
                }
            }
            else if (numTraces == 2) {
                var update = {
                    x: [[xpos], [xpos]],
                    y: [[chartData[0]], [chartData[1]]]
                }
            }
            Plotly.extendTraces(id, update, traceArray);
        }, sampleRate);
    }
    
    function getData() {
        for (i=0; i<=(numTraces-1); i++) {
            var value = document.getElementById(chartTDID[i]);
            chartData[i] = value.innerHTML;
        }      
    }
}

function removeTraces(id) {
    Plotly.deleteTraces(id, traceArray);
    if (id == 'lineChartOne') {
        chartState[0] = chartState[1] = 0;
        chartTDID[0] = chartTDID[1] = "";
        chartTitles[0] = chartTitles[1] = "";
    numTraces = 0;
}
