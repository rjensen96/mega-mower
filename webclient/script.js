const ws = new WebSocket("ws://192.168.86.181/ws");

ws.onopen = () => console.log("WebSocket connected!");
ws.onclose = () => console.log("WebSocket closed");
ws.onerror = () => console.log("WebSocket error!");
ws.onmessage = (e) => console.log("Message from websocket: ", e.data);

// BUTTONS
document
  .getElementById("btnForward")
  .addEventListener("click", handleForwardClick);
document
  .getElementById("btnReverse")
  .addEventListener("click", handleReverseClick);

function handleForwardClick() {
  ws.send("forward");
}

function handleReverseClick() {
  ws.send("reverse");
}
