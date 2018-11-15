module.exports = { 
    getData(tdID) {
        var chartOneTraceOne = document.getElementById(tdID);
        return chartOneTraceOne.innerHTML;
        
        //return Math.random(); // Placeholder until method of data access is available
    }
}
