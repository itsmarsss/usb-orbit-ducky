let currentEditing = "/";

function getStatus() {
    fetch('/api/status')
        .then(res => res.json())
        .then(out => setStatus(out))
        .catch(err => {
            console.log('Error querying json');
            alert('Error getting status... Please reload: ' + err);
            throw err;
        });
}

function setStatus(data) {
    storage_bytes.innerHTML = data.file_system_used;
    storage_percentage_free.innerHTML = (data.file_system_used / data.file_system_size).toFixed(2);
    storage_progress.value = storage_percentage_free.innerHTML;

    chip_connectivity.classList.remove("green_back");
    chip_connectivity.classList.remove("red_back");
    chip_connectivity.innerHTML = data.atmega32U4 == 0 ? "offline" : "online";
    if (data.atmega32U4 == 0) {
        chip_connectivity.classList.add("red_back");
    } else {
        chip_connectivity.classList.add("green_back");
    }
}

function getScripts() {
    fetch('/api/scripts')
        .then(res => res.json())
        .then(out => setScripts(out))
        .catch(err => {
            console.log('Error querying json');
            alert('Error getting scripts... Please reload:' + err);
            throw err;
        });
}

function setScripts(data) {
    scripts_list.innerHTML = "";
    for (let i = 0; i < data.scripts.length; i++) {
        scripts_list.innerHTML += `
    <div class="script_item" id="${data.scripts[i].name}">
          <span class="script_value">${data.scripts[i].name}</span>
          <span class="script_value">${data.scripts[i].bytes}</span>
          <div class="script_value script_actions">
            <button class="red" onclick="deleteScript('${data.scripts[i].name}')">DELETE</button>
            <button class="blue" onclick="getScript('${data.scripts[i].name}')">EDIT</button>
            <button class="yellow" onclick="runScript('${data.scripts[i].name}')">RUN</button>
          </div>
        </div>`;
    }
}

function newScript() {
    fetch('/api/new_script?file_name=' + stringToHex(new_script_input.value))
        .then(res => res.text())
        .then(out => {
            if (out == '200') {
                new_script_input.value = "/";
                getScripts();
            } else {
                alert(out);
            }
        })
        .catch(err => {
            console.log('Error querying json');
            alert('Error creating scripts... Please reload:' + err);
            throw err;
        });
}

function deleteScript(file_name) {
    fetch('/api/delete_script?file_name=' + stringToHex(file_name))
        .then(res => res.text())
        .then(out => {
            if (out == '200') {
                getScripts();
            } else {
                alert(out);
            }
        })
        .catch(err => {
            console.log('Error querying json');
            alert('Error deleting script... Please reload:' + err);
            throw err;
        });
}

function getScript(file_name) {
    fetch('/api/get_script?file_name=' + stringToHex(file_name))
        .then(res => res.json())
        .then(out => {
            if (out.status == '200') {
                currentEditing = file_name;
                script_name_input.value = file_name;
                script_editor.value = hexToString(out.content);
            } else {
                alert(out.content);
            }
        })
        .catch(err => {
            console.log('Error querying json');
            alert('Error getting script... Please reload:' + err);
            throw err;
        });
}

function saveScript() {
    if (currentEditing == "/") {
        alert("Edit a script first.");
        return;
    }

    fetch('/api/save_script?file_name=' + stringToHex(currentEditing) + '&new_file_name=' + stringToHex(script_name_input.value) + '&script=' + stringToHex(script_editor.value))
        .then(res => res.text())
        .then(out => {
            if (out != '200') {
                alert(out);
            }
            getScripts();
        })
        .catch(err => {
            console.log('Error querying json');
            alert('Error saving script... Please reload:' + err);
            throw err;
        });
}

function runScript(file_name) {
    fetch('/api/run_script?file_name=' + stringToHex(file_name))
        .then(res => res.text())
        .then(out => {
            if (out != '200') {
                alert(out);
            }
        })
        .catch(err => {
            console.log('Error querying json');
            alert('Error running script... Please reload:' + err);
            throw err;
        });
}

function downloadScript() {
    window.open(
        "./api/download_script?file_name=" + script_name_input.value);
}

function prependSlash(element) {
    let value = element.value;
    if (value.charAt(0) != "/") {
        element.value = "/" + value;
    }
}

new_script_input.addEventListener("input", () => {
    prependSlash(new_script_input);
});
script_name_input.addEventListener("input", () => {
    prependSlash(script_name_input);
});

getStatus();
getScripts();