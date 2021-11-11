const ws = new WebSocket("ws://192.168.86.181/ws");

ws.onopen = () => console.log("WebSocket connected!");
ws.onclose = () => console.log("WebSocket closed");
ws.onerror = () => console.log("WebSocket error!");
ws.onmessage = (e) => console.log("Message from websocket: ", e.data);

window.addEventListener("keydown", handleKeyDown);
window.addEventListener("keyup", handleKeyUp);
window.setInterval(sendDriveCommand, 100);

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

/* controls for WASD keys */

const controlKeysDown = {
  KeyW: false,
  KeyA: false,
  KeyS: false,
  KeyD: false,
};

function handleKeyDown(e) {
  if (Object.hasOwnProperty.call(controlKeysDown, e.code)) {
    controlKeysDown[e.code] = true;
  }
}

function handleKeyUp(e) {
  if (Object.hasOwnProperty.call(controlKeysDown, e.code)) {
    controlKeysDown[e.code] = false;
  }
}

function sendDriveCommand() {
  // todo: check if ws is open

  // idea: get array of what is pressed, and sort it.
  // then concatenate it.
  // then use those concatenated keys to figure out what to do.
  const arKeysDown = [];
  for (const key in controlKeysDown) {
    if (controlKeysDown[key] === true) {
      arKeysDown.push(key);
    }
  }

  arKeysDown.sort();

  const keysDownString = arKeysDown.join("");
  // https://www.w3schools.com/js/js_bitwise.asp
  const commands = {
    KeyW: { left: 11111111, right: 11111111 },
    KeyS: { left: 11111111, right: 11111111 },
  };

  console.log(keysDownString);

  if (controlKeysDown["KeyW"]) {
    console.log("FORWARD");
  } else if (controlKeysDown["KeyS"]) {
    console.log("REVERSE");
  } else if (controlKeysDown["KeyA"]) {
    console.log("LEFT");
  } else if (controlKeysDown["KeyD"]) {
    console.log("RIGHT");
  }
}
