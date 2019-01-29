module.exports = { 
    getData(tdID) {
        var chartOneTraceOne = document.getElementById(tdID);
        return chartOneTraceOne.innerHTML;
        
        //return Math.random(); // Placeholder until method of data access is available
    }
}
// How to turn 8 lines into 1
//
document.getElementById("Put your element here").innerHTML
//
// - Eric