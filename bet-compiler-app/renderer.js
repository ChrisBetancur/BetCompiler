const { ipcRenderer, remote } = require("electron");
const { exec } = require("child_process");
const path = require("path");
const fs = require('fs')

const { listDir } = require("./compilerWindow");
const { bet_dir_path } = require("./util");

const  {
    readFile,
    formatASTContent,
    compiler_path,
    compiler_make,
    ast_file_path,
    tokens_file_path,
    asm_file_path
} = require("./util");



window.addEventListener("DOMContentLoaded", () => {
    /*listDir(bet_dir_path) .then((result) => {
        if (result) {
            nav.innerHTML = result;
            console.log(nav.innerHTML);
        }
    }).catch((err) => {
        console.log('Error: ' + err);
    });*/


    const el = {
        fileName: document.getElementById("fileName"),
        fileNameTab: document.getElementById("fileNameTab"),
        createDocumentBtn: document.getElementById("createDocumentBtn"),
        openDocumentBtn: document.getElementById("openDocumentBtn"),
        fileTextarea: document.getElementById("fileTextarea"),
        runFile: document.getElementById("runBetFile"),
        output: document.getElementById("output"),
        astOutput: document.getElementById("astOutput"),
        tokensOutput: document.getElementById("tokenOutput"),
        asmOutput: document.getElementById("asmOutput"),
        dirList: document.getElementById("dirList"),
        betfile: document.getElementById("betfile")
    };

    const handleFileChange = (filePath, content = "") => {
        el.fileName.innerHTML = path.parse(filePath).base;
        el.fileNameTab.innerHTML = path.parse(filePath).base;
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



    listDir(bet_dir_path).then((result) => {
        if (result) {
            el.dirList.innerHTML = result;


            setTimeout(() => {
                const betfileElements = document.getElementsByClassName("betfile");
                Array.from(betfileElements).forEach((element) => {
                    element.addEventListener("click", (e) => {
                        const filePath = e.target.getAttribute("data-filepath");
                        readFile(filePath)
                            .then((fileContents) => {
                                handleFileChange(filePath, fileContents);
                            })
                            .catch((error) => {
                                console.error('Error reading file:', error);
                            });
                    });
                });
            }, 0);
        }
    })
    .catch((err) => {
        console.log('Error: ' + err);
    });

    el.fileTextarea.addEventListener("input", (e) => {
        console.log("hello\n");
        ipcRenderer.send("file-content-updated", e.target.value);
    });

    ipcRenderer.on("document-opened", (_, { filePath, content }) => {
        handleFileChange(filePath, content);
    });

    ipcRenderer.on("document-created", (_, filePath) => {
        handleFileChange(filePath);
    });

    el.runFile.addEventListener("click", (e) => {
        exec(compiler_make, { cwd: compiler_path }, (error, stdout, stderr) => {
            if (error) {
                console.error(`exec error: ${error}`);
                return;
            }
            console.log(`stdout: ${stdout}`);
            console.log(`stderr: ${stderr}`);

            console.log(fileName.innerHTML);
            exec("./output bet_files/" + fileName.innerHTML, { cwd: compiler_path }, (error, stdout, stderr) => {
                console.log(`stdout: ${stdout}`);
                                console.log(`stderr: ${stderr}`);

                const formattedOutput = stdout.replace(/\n/g, "<br>");

                el.output.innerHTML = formattedOutput;

                if (error) {
                    console.log("yo");
                    return;
                }

                readFile(ast_file_path).then((fileContents) => {
                    const formattedContents = formatASTContent(fileContents);
                    el.astOutput.innerHTML = formattedContents;
                }).catch((error) => {
                    console.error('Error reading file:', error);
                });


                readFile(tokens_file_path).then((fileContents) => {
                    const formattedContents = formatASTContent(fileContents);
                    el.tokensOutput.innerHTML = formattedContents;
                }).catch((error) => {
                    console.error('Error reading file:', error);
                });


                readFile(asm_file_path).then((fileContents) => {
                    const formattedContents = formatASTContent(fileContents);
                    el.asmOutput.innerHTML = formattedContents;
                }).catch((error) => {
                    console.error('Error reading file:', error);
                });


            });
        });
    });
});

