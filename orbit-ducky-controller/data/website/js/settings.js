function updateSettings() {

}

function restartESP() {
    console.log('Restarting...');
    fetch('/api/restart')
        .then(() => {
            alert('Restarting...');
        })
        .catch(() => {
            alert('Restarting...');
        });
}

function shutdownESP() {
    console.log('Shutting Down...');
    fetch('/api/shutdown')
        .then(() => {
            alert('Shutting Down...');
        })
        .catch(() => {
            alert('Shutting Down...');
        });
}