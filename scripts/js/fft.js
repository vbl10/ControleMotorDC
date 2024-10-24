/**
 * Uso:
 *  node fft.js
 * 
 * Opções:
 *  --caminho <caminho do arquivo a ser processado>     [obrigatório]
 *  --complexo                                          [amplitude e fase representadas com números complexos]
 * 
 */

const fs = require('fs');
const math = require('mathjs');

const args = process.argv.slice(2);
let filePath;
{
    const filePathArgIndex = args.findIndex((val) => val == '--caminho');
    if (filePathArgIndex == -1 || filePathArgIndex + 1 >= args.length)
        throw new Error("Deve ser especificado o caminho do arquivo de texto a ser processado com o argumento --caminho");
    filePath = args[filePathArgIndex + 1];
}
const complexResult = args.findIndex((val) => val == '--complexo') != -1;

fs.readFile(filePath, 'utf8', (err, data) => {
    data = data.slice(data.indexOf('\n') + 1);
    const dataset = [];
    for (
        let line = data.substring(0, data.indexOf('\n')); 
        line.length > 0; 
        line = data.substring(0, data.indexOf('\n'))
    ) {
        data = data.slice(data.indexOf('\n') + 1);
        const sample = Number.parseInt(line);
        dataset.push(sample);
    }
    let output = math.fft(dataset);

    if (complexResult) {
        output.join('\n');
    }
    else {
        output = 'Amplitude, Fase\n' + output.map(val => `${val.re}, ${val.im}`).join('\n');
    }

    const outputFilePath = `${filePath.substring(0, filePath.lastIndexOf('.'))}.fft${filePath.substring(filePath.lastIndexOf('.'))}`;
    fs.writeFile(
        outputFilePath,
        output,
        (err) => { if (err) console.log(err); }
    );
    console.log(`Resultados escritos em ${outputFilePath}`);
})