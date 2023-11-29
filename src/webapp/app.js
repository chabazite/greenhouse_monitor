// Replace with your API Gateway WebSocket URL
const wsUrl = 'wss://5zed3edbwa.execute-api.us-east-2.amazonaws.com/production/';

const ws = new WebSocket(wsUrl);

ws.onopen = () => {
    console.log('WebSocket connection established');
};

ws.onerror = (error) => {
    console.error('WebSocket error:', error);
};

ws.onmessage = (event) => {
    try {
        const data = JSON.parse(event.data);

        // Update from first Lambda function
        if(data.heat_index !== undefined) {
            document.getElementById('heat-index').textContent = data.heat_index.toFixed(2);
        }
        if(data.humidity !== undefined) {
            document.getElementById('humidity').textContent = data.humidity;
        }
        if(data.temperature !== undefined) {
            document.getElementById('temperature').textContent = data.temperature.toFixed(2);
        }
        if(data.datetime !== undefined) {
            document.getElementById('datetime').textContent = data.datetime;
        }

        // Update from second Lambda function
        if(data.mean_max !== undefined) {
            document.getElementById('prediction').textContent = data.mean_max.toFixed(2);
        }
    } catch (error) {
        console.error('Error parsing WebSocket data:', error);
    }
};
console.log("Before clock function is running");
function updateClock() {
    console.log("Clock function is running");
    const now = new Date();
    const hours = String(now.getUTCHours()).padStart(2, '0');
    const minutes = String(now.getUTCMinutes()).padStart(2, '0');
    const seconds = String(now.getUTCSeconds()).padStart(2, '0');
    document.getElementById('real-time-clock').textContent = `${hours}:${minutes}:${seconds}`;
}
console.log("before set interval clock function is running");
setInterval(updateClock, 1000);
console.log("before function is running");
updateClock();

ws.onclose = () => {
    console.log('WebSocket connection closed');
};
