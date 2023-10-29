function stringToHex(input) {
    let hex = "";
    for (let i = 0; i < input.length; i++) {
        hex += input.charCodeAt(i).toString(16).padStart(2, '0');
    }
    return hex;
}

function hexToString(hex) {
    let str = "";
    for (let i = 0; i < hex.length; i += 2) {
        const hexByte = hex.substr(i, 2);
        str += String.fromCharCode(parseInt(hexByte, 16));
    }
    return str;
}