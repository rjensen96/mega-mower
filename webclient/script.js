const ws = new WebSocket("ws://192.168.86.181/ws");

ws.onopen = () => console.log("WebSocket connected!");
ws.onclose = () => console.log("WebSocket closed");
ws.onerror = () => console.log("WebSocket error!");
ws.onmessage = (e) => console.log("Message from websocket: ", e.data);

ws.binaryType = "arraybuffer";

window.addEventListener("keydown", handleKeyDown);
window.addEventListener("keyup", handleKeyUp);
window.setInterval(sendDriveCommand, 50);

// BUTTONS
// document
//   .getElementById("btnForward")
//   .addEventListener("click", handleForwardClick);
// document
//   .getElementById("btnReverse")
//   .addEventListener("click", handleReverseClick);

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

const getKeysDownString = () => {
  // idea: get array of what is pressed, and sort it.
  // then concatenate it.
  // then use those concatenated keys to figure out what to do.
  const arKeysDown = [];
  for (const key in controlKeysDown) {
    if (controlKeysDown[key] === true) {
      arKeysDown.push(key);
    }
  }

  return arKeysDown.sort().join("");
};

function sendDriveCommand() {
  // todo: check if ws is open

  const commands = {
    // 8 directions in 45 degree increments.
    // this is so that we can fit the whole payload into a Uint8.

    // signular commands
    KeyD: 1,
    KeyDKeyW: 2,
    KeyW: 3,
    KeyAKeyW: 4,
    KeyA: 5,
    KeyAKeyS: 6,
    KeyS: 7,
    KeyDKeyS: 8,
  };

  const keysDownString = getKeysDownString();

  if (Object.hasOwnProperty.call(commands, keysDownString)) {
    const buffer = new Uint8Array([commands[keysDownString]]).buffer;
    ws.send(buffer);
  }
}
