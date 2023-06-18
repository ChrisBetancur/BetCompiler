
const {
  BrowserWindow,
  app,
  ipcMain,
  dialog,
  Notification,
  Menu,
} = require("electron");

const path = require("path");
const fs = require("fs");

const { createCompilerWindow } = require("./compilerWindow");

const isDevEnv = process.env.NODE_ENV === "development";

if (isDevEnv) {
  try {
    require("electron-reloader")(module);
  } catch {}
}

let compilerWindow;
let openedFilePath;

const initCompilerWindow = (isDevEnv) => {
    compilerWindow = createCompilerWindow();
};

app.whenReady().then(initCompilerWindow);

const handleError = () => {
  new Notification({
    title: "Error",
    body: "Sorry, something went wrong :(",
  }).show();
};



const openFile = (window, filePath) => {
  fs.readFile(filePath, "utf8", (error, content) => {
    if (error) {
      handleError();
    } else {
      app.addRecentDocument(filePath);
      openedFilePath = filePath;
      window.webContents.send("document-opened", { filePath, content });
    }
  });
};

app.on("open-file", (_, filePath) => {
  openFile(compilerWindow, filePath);
});

ipcMain.on("open-document-triggered", () => {
    const appDir = path.dirname(__dirname);
    const defaultDir = path.join(appDir, "bet_files");

    dialog
    .showOpenDialog({
      properties: ["openFile"],
      filters: [{ name: "bet files", extensions: ["bet"] }],
        defaultPath: defaultDir,
    })
    .then(({ filePaths }) => {
      const filePath = filePaths[0];

      openFile(compilerWindow, filePath);
    });
});

/*
ipcMain.on("create-document-triggered", () => {
    const appDir = path.dirname(__dirname);
    const defaultDir = path.join(appDir, "bet_files");

  dialog
    .showSaveDialog(compilerWindow, {
      filters: [{ name: "bet files", extensions: ["bet"] }],
        defaultPath: defaultDir,
    })
    .then(({ filePath }) => {
      fs.writeFile(filePath, "", (error) => {
        if (error) {
          handleError();
        } else {
          app.addRecentDocument(filePath);
          openedFilePath = filePath;
          compilerWindow.webContents.send("document-created", filePath);
        }
      });
    });
});*/

ipcMain.on("open-document-triggered", () => {
  const appDir = path.dirname(__dirname);
  const defaultDir = path.join(appDir, "bet_files");

  dialog
    .showOpenDialog({
      properties: ["openFile"],
      filters: [{ name: "bet files", extensions: ["bet"] }],
      defaultPath: defaultDir,
    })
    .then(({ filePaths }) => {
      const filePath = filePaths[0];

      if (filePath) {
        openFile(compilerWindow, filePath);
      }
    })
    .catch((error) => {
      console.error("Error while opening the file:", error);
    });
});

/*
ipcMain.on("file-content-updated", (_, textareaContent) => {
  fs.writeFile(openedFilePath, textareaContent, (error) => {
    if (error) {
      handleError();
    }
  });
});*/



ipcMain.on("file-content-updated", (_, textareaContent) => {
    fs.writeFile(openedFilePath, textareaContent, (error) => {
        console.log("balls\n\n\n\n");
        if (error) {
            //handleError();
            console.log(error);
        }
    });
});

