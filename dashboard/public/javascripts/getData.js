module.exports = { 
    getData(tdID) {
        var chartOneTraceOne = document.getElementById(tdID);
        return chartOneTraceOne.innerHTML;
        
    }
}
