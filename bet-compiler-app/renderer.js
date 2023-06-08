
const { ipcRenderer } = require("electron");
const { exec } = require("child_process");
//const { spawn } = require("child_process");
const path = require("path");
const compiler_path = "/home/c_bet/Projects/BetCompiler/";
const compiler_make = "make run";
const ast_file_path = compiler_path + "abstract_syntax_tree.txt";
const tokens_file_path = compiler_path + "tokens.txt";
const asm_file_path = compiler_path + "output.asm";

const fs = require("fs");

function readFile(filePath) {
    return new Promise((resolve, reject) => {
        fs.readFile(filePath, "utf8", (err, data) => {
            if (err) {
                reject(err);
            } else {
                resolve(data);
            }
        });
    });
}

function formatASTContent(content) {
  const lines = content.split('\n');
  const formattedLines = lines.map((line) => {
    const indentation = line.match(/^\s*/)[0];
    const formattedLine = line.trim().replace(/</g, '&lt;').replace(/>/g, '&gt;');
    return `${indentation}${formattedLine}`;
  });
  return formattedLines.join('<pre>');
}

window.addEventListener("DOMContentLoaded", () => {
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

    el.fileTextarea.addEventListener("input", (e) => {
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

            exec("./output bet_files/" + fileName.innerHTML, { cwd: compiler_path }, (error, stdout, stderr) => {
                if (error) {
                    console.error(`exec error: ${error}`);
                    return;
                }
                console.log(`stdout: ${stdout}`);
                console.log(`stderr: ${stderr}`);

                const formattedOutput = stdout.replace(/\n/g, "<br>");

                el.output.innerHTML = formattedOutput;

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


