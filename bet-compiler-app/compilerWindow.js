
const { BrowserWindow, app, ipcMain, Menu } = require("electron");
const path = require("path");

const isDevEnv = process.env.NODE_ENV === "development";

const createCompilerWindow = () => {
    compilerWindow = new BrowserWindow({
        width: 1280,
        height: 720,
        titleBarStyle: "hiddenInset",
        webPreferences: {
            devTools: true,
            sandbox: false,
            preload: path.join(app.getAppPath(), "renderer.js"),
        },
    });

    if (isDevEnv) {
        compilerWindow.webContents.openDevTools();
    }

    compilerWindow.loadFile("index.html");

    const menuTemplate = [
        {
          label: "File",
          submenu: [
            {
              label: "Add New File",
              click: () => ipcMain.emit("open-document-triggered"),
            },
            {
              label: "Create New File",
              click: () => ipcMain.emit("create-document-triggered"),
            },
            { type: "separator" },
            {
              label: "Open Recent",
              role: "recentdocuments",
              submenu: [
                {
                  label: "Clear Recent",
                  role: "clearrecentdocuments",
                },
              ],
            },
            {
              role: "quit",
            },
          ],
        },
        {
          label: "Edit",
          submenu: [
            { role: "undo" },
            { role: "redo" },
            { type: "separator" },
            { role: "cut" },
            { role: "copy" },
            { role: "paste" },
            { role: "pasteAndMatchStyle" },
            { role: "delete" },
            { role: "selectAll" },
            { type: "separator" },
            {
              label: "Speech",
              submenu: [{ role: "startSpeaking" }, { role: "stopSpeaking" }],
            },
          ],
        },
      ];

    const menu = Menu.buildFromTemplate(menuTemplate);
    Menu.setApplicationMenu(menu);

    return compilerWindow;
};

module.exports = {
    createCompilerWindow
};

