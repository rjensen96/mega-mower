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
    // list of DEGREES representing DIRECTION vehicle should move.
    // consult the unit circle if you have forgotten how degrees work.
    // 90 is pure forward; 270 is pure reverse.

    // signular commands
    KeyW: 90,
    KeyA: 180,
    KeyS: 270,
    KeyD: 0,

    // diagonals
    KeyAKeyW: 135,
    KeyDKeyW: 45,
    KeyAKeyS: 225,
    KeyDKeyS: 315,
  };

  const keysDownString = getKeysDownString();

  if (Object.hasOwnProperty.call(commands, keysDownString)) {
    console.log("command:", commands[keysDownString]);

    const buffer = new Uint16Array([commands[keysDownString]]).buffer;
    console.log("buffer:", buffer);
    const view = new Int16Array(buffer);
    console.log("view:", view);
    ws.send(commands[keysDownString]);
  }
}
