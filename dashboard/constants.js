module.exports =  {
    serverAddr: {
        ip : "127.0.0.1",
        port : 33333
    },
    databaseAddr: {
       ip: "127.0.0.1",
       port: 27017
    },
    scanningRate : 300,
    mins:{
       off:{},
       idle:{},
       readyForPumpdown:{},
       pumpdown:{},
       ready:{},
       postRun:{},
       crawl:{}
    },
    maxs:{
        off:{},
        idle:{},
        readyForPumpdown:{},
        pumpdown:{},
        ready:{},
        postRun:{},
        crawl:{}
     }  
}