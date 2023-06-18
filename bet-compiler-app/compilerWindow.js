
const { BrowserWindow, app, ipcMain, Menu } = require("electron");
const path = require("path");
const fs = require("fs").promises;
const { bet_dir_path } = require("./util");

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

/*function listDir(curr_dir, html) {
    fs.readdir(curr_dir, function (err, files) {
        if (err) {
            return console.log('Unable to scan directory: ' + err);
        }
        console.log(curr_dir);
        if (html == null)
            html = "<ul>\n";
        else
            html += "<ul>\n";
        files.forEach(function (file) {
            let curr_path = curr_dir + "/" + file;
            if (fs.lstatSync(curr_path).isDirectory()) {
                html += "\t<li><a href=\"#\">" + file + "</a>\n";
                html += "\t\t<ul class=\"dropdown\">\n";
                listDir(curr_path, function (subHtml) {
                    html += subHtml;
                    html += "\t\t</ul>\n";
                    html += "\t</li>\n";
                });
            } else {
                html += "\t<li><a href=\"#\">" + file + "</a></li>\n";
            }
        });
        html += "</ul>\n";
        return html;
    });
}*/

async function listDir(curr_dir) {
    try {
        const files = await fs.readdir(curr_dir);
        let html = "<ul class=\"directory-list\">\n";

        for (const file of files) {
            const curr_path = curr_dir + "/" + file;
            const stats = await fs.stat(curr_path);

            if (stats.isDirectory()) {
                html += "\t<li><a href=\"#\"><img src=\"images/dir.png\" width=\"15\" height=\"15\" /> " + file + "&dtrif;</a>\n";
                html += "\t\t<ul class=\"dropdown\">\n";
                const subHtml = await listDir(curr_path);
                html += subHtml;
                html += "\t\t</ul>\n";
                html += "\t</li>\n";
            } else {
                html += "\t<li><a id=\"betfile\" class=\"betfile\" data-filepath=\"" + curr_path + "\" href=\"#\"> <img src=\"images/betfile.png\" width=\"18\" height=\"18\" /> " + file + "</a></li>\n";
            }
        }

        html += "</ul>\n";
        return html;
    } catch (err) {
        console.log('Unable to scan directory: ' + err);
        return null;
    }
}


module.exports = {
    createCompilerWindow,
    listDir
};

