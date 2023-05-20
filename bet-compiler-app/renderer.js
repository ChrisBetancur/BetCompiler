
const { ipcRenderer } = require("electron");
const { exec } = require("child_process")
//const { spawn } = require("child_process")
const path = require("path")
const makefile_path = "/home/c_bet/Projects/BetCompiler"
const compiler_make = "make run"


window.addEventListener("DOMContentLoaded", () => {
  const el = {
    documentName: document.getElementById("documentName"),
    createDocumentBtn: document.getElementById("createDocumentBtn"),
    openDocumentBtn: document.getElementById("openDocumentBtn"),
    fileTextarea: document.getElementById("fileTextarea"),
    runFile: document.getElementById("runBetFile"),
    output: document.getElementById("output")
  };

  const handleDocumentChange = (filePath, content = "") => {
    el.documentName.innerHTML = path.parse(filePath).base;
    el.fileTextarea.removeAttribute("disabled");
    el.fileTextarea.value = content;
    el.fileTextarea.focus();
  };

    el.createDocumentBtn.addEventListener("click", () => {
        ipcRenderer.send("create-document-triggered");
    });

    el.openDocumentBtn.addEventListener("click", () => {
        ipcRenderer.send("open-document-triggered");
    });

    el.fileTextarea.addEventListener("input", (e) => {
        ipcRenderer.send("file-content-updated", e.target.value);
    });

    ipcRenderer.on("document-opened", (_, { filePath, content }) => {
        handleDocumentChange(filePath, content);
    });

    ipcRenderer.on("document-created", (_, filePath) => {
        handleDocumentChange(filePath);
    });

    el.runFile.addEventListener("click", (e) => {
        exec(compiler_make, {cwd: makefile_path}, (error, stdout, stderr) => {
            if (error) {
            console.error(`exec error: ${error}`)
            return
        }
        console.log(`stdout: ${stdout}`)
        console.log(`stderr: ${stderr}`)

        exec("./output examples/test.bet", {cwd: makefile_path}, (error, stdout, stderr) => {
            if (error) {
                console.error(`exec error: ${error}`)
                return
            }
                console.log(`stdout: ${stdout}`)
                console.log(`stderr: ${stderr}`)


            const formattedOutput = stdout.replace(/\n/g, "<br>");

            el.output.innerHTML = formattedOutput;            })
        })
    })
})

