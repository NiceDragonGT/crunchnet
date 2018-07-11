/*
index.js: CrunchNet Dashboard entry point
Copyright (c) 2018 Kade Burnside
*/

/*
This file is part of CrunchNet.

CrunchNet is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

CrunchNet is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with CrunchNet. If not, see <http://www.gnu.org/licenses/>.
*/

// Imports modules
const electron = require('electron');
const url = require('url');
const path = require('path');

var mainWindow; // Window

// Listen for app to be ready
electron.app.on('ready', function () {
    mainWindow = new electron.BrowserWindow({
        width: 450,
        height: 500,
        resizable: true,
        backgroundColor: '#ffb6c1',
        icon: path.join(__dirname, 'favicon.ico')
    }); // Creates window
    mainWindow.setMenu(null); // Removes menu from window
    // Loads HTML into window
    mainWindow.loadURL(url.format({
        pathname: path.join(__dirname, 'main.html'),
        protocol: 'file:',
        slashes: true
    }));
});
