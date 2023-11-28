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
        // Update the web page with new data
        document.getElementById('heat-index').textContent = data.heat_index.toFixed(2);
        document.getElementById('humidity').textContent = data.humidity;
        document.getElementById('temperature').textContent = data.temperature.toFixed(2);
        document.getElementById('datetime').textContent = data.datetime;
        document.getElementById('prediction').textContent = data.prediction.toFixed(2);
    } catch (error) {
        console.error('Error parsing WebSocket data:', error);
    }
};

ws.onclose = () => {
    console.log('WebSocket connection closed');
};
