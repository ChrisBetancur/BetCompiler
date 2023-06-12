const { ipcRenderer } = require("electron");
const path = require("path");

const {
    readFile,
    formatASTContent,
    compiler_path,
    compiler_make,
    ast_file_path,
    tokens_file_path,
    asm_file_path
} = require("./util");

const handleFileChange = (filePath, content = "") => {
    el.fileName.innerHTML = path.parse(filePath).base;
    el.fileNameTab.innerHTML = path.parse(filePath).base;
    el.fileTextarea.removeAttribute("disabled");
    el.fileTextarea.value = content;
    el.fileTextarea.focus();
};


const compilerEventHandler = (el) => {
    el.createDocumentBtn.addEventListener("click", () => {
        console.log("Yo\n");
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
                console.log(`stdout: ${stdout}`);
                                console.log(`stderr: ${stderr}`);

                const formattedOutput = stdout.replace(/\n/g, "<br>");

                el.output.innerHTML = formattedOutput;

                if (error) {
                    console.error(`exec error: ${error}`);
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
}
module.exports = {
    compilerEventHandler
};
