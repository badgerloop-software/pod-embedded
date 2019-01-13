const electron = require('electron');
const url = require('url');
var path = require('path');

var app = electron.app;
const BrowserWindow = electron.BrowserWindow;
let win;

function createWindow(){
  win = new BrowserWindow({width: 1920, height: 1080, webPreferences:{webSecurity: true}});

  win.loadURL(url.format({
    pathname: path.join(__dirname, 'index.html'),
    protocol: 'file:',
    slashes: true,
  }));
  
  win.on('closed', () =>{
    win = null;
  });
}

app.on('ready', createWindow);

app.on('window-all-closed', () => {
  if(process.platform !== 'darwin'){
    app.quit();
  }
});

app.on('activate', () =>{
  if(win === null){
    createWindow();
  }
});