const fs = require('fs');

const args = process.argv.slice(2);
const alpha = args[0];
const filePath = args[1];

fs.readFile(filePath, 'utf8', (err, data) => {
    data = data.slice(data.indexOf('\n') + 1);
    const output = [];
    let accumulator = 0;
    for (
        let line = data.substring(0, data.indexOf('\n')); 
        line.length > 0; 
        line = data.substring(0, data.indexOf('\n'))
    ) {
        data = data.slice(data.indexOf('\n') + 1);
        const sample = Number.parseInt(line);
        accumulator = sample * alpha + accumulator * (1 - alpha);
        output.push(Math.floor(accumulator * 100) / 100);
    }
    const outputFilePath = `${filePath.substring(0, filePath.lastIndexOf('.'))}.filtered${filePath.substring(filePath.lastIndexOf('.'))}`;
    fs.writeFile(
        outputFilePath,
        output.join('\n'),
        (err) => { if (err) console.log(err); }
    );
    console.log(`Resultados escritos em ${outputFilePath}`);
})