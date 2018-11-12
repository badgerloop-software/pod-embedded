module.exports = { 
    getData() {
        for (i=0; i<=3; i++) {
            var value = document.getElementById(chartTDID[i]);
            chartData[i] = value.innerHTML;
        }     
        return chartData;
    }
}
