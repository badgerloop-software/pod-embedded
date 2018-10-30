//module.exports = {
function generateLineChart(id, title, chartType) {
    var data = require("./public/javascripts/getData");
    var xmax = 1000; //maximum x-axis range in seconds. Should be the total runtime

    var layout = {
    xaxis: {range: [0, xmax]},
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

    document.getElementById("div1").style.height = "400px"
    document.getElementById("div1").setAttribute("id", id);

    Plotly.newPlot(id, [{
        y:[data.getData()],
        type: chartType
    }], layout);

    var sampleRate = 50; // sample rate in ms per sample

    setInterval(function(){
        var currentData = data.getData();
        Plotly.extendTraces(id,{ y:[[currentData]]}, [0]);
    }, sampleRate);
}
//}

    