#ifndef WEBSOCKET_HTML_H
#define WEBSOCKET_HTML_H

const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>ESP32 WebSocket</title>
<style>
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

body {
  font-family: Arial, sans-serif;
  background: #f0f2f5;
  padding: 15px;
}

.container {
  max-width: 600px;
  margin: 0 auto;
  background: white;
  border-radius: 12px;
  box-shadow: 0 2px 10px rgba(0,0,0,0.1);
  overflow: hidden;
}

.header {
  background: #4CAF50;
  color: white;
  padding: 20px;
  text-align: center;
}

.header h1 {
  font-size: 22px;
  font-weight: normal;
}

.status {
  padding: 15px;
  text-align: center;
  font-size: 15px;
  font-weight: bold;
  background: #fff3cd;
  color: #856404;
  border-bottom: 1px solid #e0e0e0;
}

.status.connected {
  background: #d4edda;
  color: #155724;
}

.section {
  padding: 20px;
  border-bottom: 1px solid #e0e0e0;
}

.section:last-child {
  border-bottom: none;
}

.section-title {
  font-size: 14px;
  color: #666;
  margin-bottom: 10px;
}

.input-row {
  display: flex;
  gap: 10px;
}

input[type="text"] {
  flex: 1;
  padding: 12px;
  border: 1px solid #ddd;
  border-radius: 6px;
  font-size: 15px;
}

input[type="text"]:focus {
  outline: none;
  border-color: #4CAF50;
}

button {
  padding: 12px 20px;
  border: none;
  border-radius: 6px;
  font-size: 15px;
  cursor: pointer;
  font-weight: bold;
}

.btn-primary {
  background: #4CAF50;
  color: white;
}

.btn-primary:active {
  background: #45a049;
}

.btn-danger {
  background: #f44336;
  color: white;
}

.btn-danger:active {
  background: #da190b;
}

.btn-secondary {
  background: #2196F3;
  color: white;
}

.btn-secondary:active {
  background: #0b7dda;
}

.quick-btns {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 8px;
  margin-top: 10px;
}

.messages {
  background: #fafafa;
  height: 300px;
  overflow-y: auto;
  padding: 15px;
  border-radius: 6px;
}

.msg {
  padding: 8px 12px;
  margin-bottom: 8px;
  border-radius: 6px;
  font-size: 14px;
  line-height: 1.5;
}

.msg-sent {
  background: #e3f2fd;
  color: #1565c0;
  margin-left: 15%;
}

.msg-received {
  background: #e8f5e9;
  color: #2e7d32;
  margin-right: 15%;
}

.msg-info {
  background: #fff;
  color: #666;
  text-align: center;
  font-size: 13px;
}

.time {
  font-size: 11px;
  opacity: 0.7;
  margin-right: 5px;
}

.footer {
  text-align: center;
  padding: 15px;
  background: #f9f9f9;
  color: #666;
  font-size: 12px;
}

.footer a {
  color: #4CAF50;
  text-decoration: none;
}

@media (max-width: 600px) {
  .container {
    border-radius: 0;
  }
  
  .header h1 {
    font-size: 20px;
  }
  
  .messages {
    height: 250px;
  }
  
  .quick-btns {
    grid-template-columns: repeat(2, 1fr);
  }
  
  .msg-sent {
    margin-left: 10%;
  }
  
  .msg-received {
    margin-right: 10%;
  }
}
</style>
</head>
<body>
<div class="container">

<div class="header">
  <h1>ESP32 WebSocket</h1>
</div>

<div id="status" class="status">Disconnected</div>

<div class="section">
  <button onclick="toggleConnection()" id="connBtn" class="btn btn-primary" style="width:100%">Connect</button>
</div>

<div class="section">
  <div class="section-title">Send Message</div>
  <div class="input-row">
    <input type="text" id="msgInput" placeholder="Type message...">
    <button onclick="sendMsg()" class="btn btn-secondary">Send</button>
  </div>
  <div class="quick-btns">
    <button onclick="sendQuick('ping')" class="btn btn-secondary">Ping</button>
    <button onclick="sendQuick('hello')" class="btn btn-secondary">Hello</button>
    <button onclick="sendQuick('time')" class="btn btn-secondary">Time</button>
    <button onclick="sendQuick('led on')" class="btn btn-primary">LED ON</button>
    <button onclick="sendQuick('led off')" class="btn btn-danger">LED OFF</button>
    <button onclick="clearMsg()" class="btn btn-secondary">Clear</button>
  </div>
</div>

<div class="section">
  <div class="section-title">Messages</div>
  <div id="messages" class="messages"></div>
</div>

<div class="footer">
  Powered by ESP32 &amp; <a href="https://diyables.io" target="_blank">DIYables</a>
</div>

</div>

<script>
var ws = null;
var connected = false;
var wsUrl = 'ws://' + window.location.hostname + ':81';

function toggleConnection() {
  connected ? disconnect() : connect();
}

function connect() {
  addMsg('Connecting...', 'info');
  ws = new WebSocket(wsUrl);
  
  ws.onopen = function() {
    connected = true;
    updateStatus();
    addMsg('Connected!', 'info');
  };
  
  ws.onmessage = function(e) {
    addMsg(e.data, 'received');
  };
  
  ws.onclose = function() {
    connected = false;
    updateStatus();
    addMsg('Disconnected', 'info');
  };
  
  ws.onerror = function() {
    addMsg('Connection error', 'info');
  };
}

function disconnect() {
  if (ws) ws.close();
}

function sendMsg() {
  var input = document.getElementById('msgInput');
  var msg = input.value.trim();
  if (!msg) return;
  
  if (connected && ws) {
    ws.send(msg);
    addMsg(msg, 'sent');
    input.value = '';
  } else {
    addMsg('Not connected!', 'info');
  }
}

function sendQuick(msg) {
  if (connected && ws) {
    ws.send(msg);
    addMsg(msg, 'sent');
  } else {
    addMsg('Not connected!', 'info');
  }
}

function addMsg(text, type) {
  var div = document.createElement('div');
  var time = new Date().toLocaleTimeString();
  
  if (type === 'sent') {
    div.className = 'msg msg-sent';
    div.innerHTML = '<span class="time">' + time + '</span>You: ' + text;
  } else if (type === 'received') {
    div.className = 'msg msg-received';
    div.innerHTML = '<span class="time">' + time + '</span>ESP32: ' + text;
  } else {
    div.className = 'msg msg-info';
    div.innerHTML = '<span class="time">' + time + '</span>' + text;
  }
  
  var box = document.getElementById('messages');
  box.appendChild(div);
  box.scrollTop = box.scrollHeight;
}

function clearMsg() {
  document.getElementById('messages').innerHTML = '';
}

function updateStatus() {
  var status = document.getElementById('status');
  var btn = document.getElementById('connBtn');
  
  if (connected) {
    status.textContent = 'Connected';
    status.className = 'status connected';
    btn.textContent = 'Disconnect';
    btn.className = 'btn btn-danger';
  } else {
    status.textContent = 'Disconnected';
    status.className = 'status';
    btn.textContent = 'Connect';
    btn.className = 'btn btn-primary';
  }
}

document.getElementById('msgInput').addEventListener('keypress', function(e) {
  if (e.key === 'Enter') sendMsg();
});

window.onload = function() {
  addMsg('Ready to connect', 'info');
};
</script>
</body>
</html>
)rawliteral";

#endif
