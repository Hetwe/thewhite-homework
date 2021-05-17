const fs = require('fs');
const path = require('path');

let src;
let outputPath;
let delimiter;

let arrayDyblicate = [];
let arrayElement = [
{
    name: null,
    dyplicate: null,
}
];

function deleteUUID(element, isDirectory){
    if(isDirectory){
        return element.substring(0, element.length - 37);
    }else{
        let type;
        for (let index = 0; index < element.length; index++) {
            if(element[index] == '.'){
                type = element.substring(index, element.length);
                break;
            }
        }
        return element.substring(0, element.length - type.length - 37) + type;
    }
}

function GetDirectoryAndFile(outputPath){
    try {
        let items = fs.readdirSync(src);
        for (let index = 0; index < items.length; index++) {
            let element = items[index];
            let pathToElement = src + delimiter + element;
            if(fs.lstatSync(pathToElement).isDirectory()){
                let nameFolder = deleteUUID(element, true)
                for (let index = 0; index < arrayElement.length; index++) {
                    if(nameFolder == arrayElement[index].name){
                        arrayElement[index].name = nameFolder;
                        arrayElement[index].dyplicate += 1;
                        nameFolder += ' (' + (arrayElement[index].dyplicate).toString() + ')';
                        arrayDyblicate.push(nameFolder);
                        break;
                    }
                    else if(index == arrayElement.length - 1){
                        arrayElement.push({name: nameFolder, dyplicate: 0 });
                        break;
                    }
                }
                fs.mkdirSync(outputPath + nameFolder);

                fs.readdir(pathToElement, (error, items) => {
                    for (let index = 0; index < items.length; index++) {
                        let file = items[index];
                        fs.createReadStream(pathToElement + delimiter + file).pipe(fs.createWriteStream(outputPath + nameFolder + delimiter + file));
                    }
                });
            }else{
                let newNameFile = deleteUUID(element, false)
                for (let i = 0; i < arrayElement.length; i++) {
                    if(newNameFile == arrayElement[i].name){
                        arrayElement[i].name = newNameFile;
                        arrayElement[i].dyplicate += 1;
                        for (let j = 0; j < newNameFile.length; j++) {
                            if(newNameFile[j] == '.'){
                                newNameFile = newNameFile.slice(0, j) + ' (' + (arrayElement[j].dyplicate).toString() + ')' + newNameFile.slice(j);
                                arrayDyblicate.push(newNameFile);
                                break;
                            }
                        }
                        break;
                    }
                    else if(i == arrayElement.length - 1){
                        arrayElement.push({name: newNameFile, dyplicate: 0 });
                        break;
                    }
                }
                fs.createReadStream(pathToElement).pipe(fs.createWriteStream(outputPath + newNameFile));
            }
        }
    } catch (error) {
        console.log(error);
    }
}

function makeFolder(dir){
    if(fs.existsSync(outputPath)){
        outputPath = outputPath + dir + delimiter;
        fs.mkdirSync(outputPath);
    }else{
        fs.mkdirSync(outputPath);
        outputPath = outputPath + dir + delimiter;
        fs.mkdirSync(outputPath);
    }
}

function main(){
    process.argv.forEach((value, index) => {
        if(index === 2){
            src = value;
        }else if(index === 3){
            outputPath = value;
        }   
    });
    
    if(process.platform == 'win32'){
        delimiter = '\\';
    }else{
        delimiter = '/'
    }
    if(src[src.length - 1] == '/' || src[src.length - 1] == '\\'){
        src = src.substring(0, src.length - 1);
    }

    if(path.isAbsolute(outputPath)){
        outputPath += delimiter;
    }else{
        outputPath = __dirname + delimiter + outputPath + delimiter;
    }
    let copyOutputPath = outputPath;

    makeFolder("Utrom's secrets(*)");
    GetDirectoryAndFile(outputPath);

    outputPath = copyOutputPath;

    makeFolder("Utrom's secrets");
    arrayDyblicate.splice(0, arrayDyblicate.length);
    arrayElement.splice(1, arrayElement.length);
    GetDirectoryAndFile(outputPath);
    
    fs.readdir(outputPath, (error, items) => {
        for (let index = 0; index < items.length; index++) {
            let element = items[index];
            let pathToElement = outputPath + delimiter + element;
            for (let index = 0; index < arrayDyblicate.length; index++) {
                const dyplicate = arrayDyblicate[index];
                if(element == dyplicate && fs.lstatSync(pathToElement).isDirectory()){
                    try {
                        let files = fs.readdirSync(pathToElement);
                        for (let index = 0; index < files.length; index++) {
                            const file = files[index];
                            let pathToFile = pathToElement + delimiter + file;
                            fs.unlinkSync(pathToFile);
                        }
                        fs.rmdirSync(pathToElement);
                    } catch (error) {
                        console.log(error);
                    }
                }else if(element == dyplicate && fs.lstatSync(pathToElement).isFile()){
                    fs.unlinkSync(pathToElement);
                }
            }
        }
    });
}

main();
