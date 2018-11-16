function generateBlankChart(id, title, chartType) {
    var xmax = 100; //maximum x-axis range in seconds. Should be the total runtime

    var layout = {
    xaxis: {range: [0, xmax]},
    title: title,
    width: 220,
    height: 220,
    margin: {
        l: 30,
        r: 20,
        b: 20,
        t: 30,
        }
    };

    Plotly.newPlot(id, [{
        y:[],
        type: chartType
    }], layout);
}