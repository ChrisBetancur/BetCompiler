const fs = require("fs");
const compiler_path = "/home/c_bet/Projects/BetCompiler/";
const compiler_make = "make run";
const ast_file_path = compiler_path + "abstract_syntax_tree.txt";
const tokens_file_path = compiler_path + "tokens.txt";
const asm_file_path = compiler_path + "output.asm";



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

module.exports = {
    readFile,
    formatASTContent,
    compiler_path,
    compiler_make,
    ast_file_path,
    tokens_file_path,
    asm_file_path
};
