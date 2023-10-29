function getSettings() {
    fetch('/api/settings')
        .then(res => res.json())
        .then(out => setSettings(out))
        .catch(err => {
            console.log('Error querying json');
            alert('Error getting settings... Please reload: ' + err);
            throw err;
        });
}

function setSettings(data) {
    ssid_input.value = data.ssid;
    password_input.value = data.pwd;

    channel_input.value = data.channel;
    hide_ssid_input.value = data.hidessid;
    max_connect_input.value = data.maxconnect;
    ftm_respond_input.value = data.ftmrespond;

    ip_1.value = data.ip1;
    ip_2.value = data.ip2;
    ip_3.value = data.ip3;
    ip_4.value = data.ip4;

    gateway_1.value = data.gateway1;
    gateway_2.value = data.gateway2;
    gateway_3.value = data.gateway3;
    gateway_4.value = data.gateway4;

    subnet_1.value = data.subnet1;
    subnet_2.value = data.subnet2;
    subnet_3.value = data.subnet3;
    subnet_4.value = data.subnet4;
}

function updateSettings() {
    let settings = `
ssid=${stringToHex(ssid_input.value)}
&pwd=${stringToHex(password_input.value)}
&channel=${channel_input.value}
&hidessid=${hide_ssid_input.value}
&maxconnect=${max_connect_input.value}
&ftmrespond=${ftm_respond_input.value}
&ip1=${ip_1.value}
&ip2=${ip_2.value}
&ip3=${ip_3.value}
&ip4=${ip_4.value}
&gateway1=${gateway_1.value}
&gateway2=${gateway_2.value}
&gateway3=${gateway_3.value}
&gateway4=${gateway_4.value}
&subnet1=${subnet_1.value}
&subnet2=${subnet_2.value}
&subnet3=${subnet_3.value}
&subnet4=${subnet_4.value}`

    fetch('/api/update_settings?' + settings)
        .then(res => res.text())
        .then(out => {
            if (out == '200') {
                getSettings();
            } else {
                alert(out);
            }
        })
        .catch(err => {
            console.log('Error querying json');
            alert('Error updating settings... Please reload:' + err);
            throw err;
        });
}

function restartESP() {
    fetch('/api/restart')
        .then(() => {
            alert('Restarting...');
        })
        .catch(() => {
            alert('Restarting...');
        });
}

function shutdownESP() {
    fetch('/api/shutdown')
        .then(() => {
            alert('Shutting Down...');
        })
        .catch(() => {
            alert('Shutting Down...');
        });
}

getSettings();