/* WEBSOCKET SETUP */

let wsIsOpen = false;

const ws = new WebSocket("ws://192.168.86.181/ws");

ws.onopen = handleWsOpen;
ws.onclose = handleWsClose;
ws.onerror = () => console.log("WebSocket error!");
ws.onmessage = handleWsMessage;

ws.binaryType = "arraybuffer";

function handleWsOpen() {
  console.log("WebSocket connected");
  wsIsOpen = true;
  setConnectedState(true);
}

function handleWsClose() {
  console.log("WebSocket closed");
  wsIsOpen = false;
  setConnectedState(false);
}

function handleWsMessage(e) {
  console.log("Message from websocket: ", e.data);
  // todo: consider preventing new messages from being sent if echoes don't return in good time.
  // a problem is that the WebSocket gets clogged with commands, ESP32 stalls, then when you least expect it...
  // every single command goes through and your lawnmower runs over the neighbor's cat.
  // we would prefer to prevent big backlogs from accumulating like that
}

/* GLOBAL EVENT HANDLERS */

window.addEventListener("keydown", handleKeyDown);
window.addEventListener("keyup", handleKeyUp);
window.setInterval(sendDriveCommand, 50);

/* CLIENT STATE MANAGEMENT */

function setConnectedState(isConnected) {
  const connectionText = isConnected ? "Connected" : "Disconnected";
  const iconText = isConnected ? "check_circle" : "cancel";
  const iconColor = isConnected ? "limegreen" : "red";
  document.getElementById("connectionStatus").innerText = connectionText;
  document.getElementById("connectionIcon").innerText = iconText;
  document.getElementById("connectionIcon").style.color = iconColor;
}

/* CONTROLS FOR WASD KEYS */

const controlKeysDown = {
  KeyW: false,
  KeyA: false,
  KeyS: false,
  KeyD: false,
};

function handleKeyDown(e) {
  if (Object.hasOwnProperty.call(controlKeysDown, e.code)) {
    controlKeysDown[e.code] = true;
    document.getElementById(e.code).className = "keychipDown";
  }
}

function handleKeyUp(e) {
  if (Object.hasOwnProperty.call(controlKeysDown, e.code)) {
    controlKeysDown[e.code] = false;
    document.getElementById(e.code).className = "keychipUp";
  }
}

const getKeysDownString = () => {
  // get array of what is pressed, then sort & concatenate it
  // elsewhere use concatenated key to figure out what to do.
  const arKeysDown = [];
  for (const key in controlKeysDown) {
    if (controlKeysDown[key] === true) {
      arKeysDown.push(key);
    }
  }

  return arKeysDown.sort().join("");
};

function sendDriveCommand() {
  if (!wsIsOpen) {
    return;
  }

  const commands = {
    // 8 directions in 45 degree increments.
    // this is so that we can fit the whole payload into a Uint8. (could even be a single byte)
    empty: 0,
    KeyD: 1,
    KeyDKeyW: 2,
    KeyW: 3,
    KeyAKeyW: 4,
    KeyA: 5,
    KeyAKeyS: 6,
    KeyS: 7,
    KeyDKeyS: 8,
  };

  const keysDownString = getKeysDownString() || "empty";

  if (Object.hasOwnProperty.call(commands, keysDownString)) {
    const buffer = new Uint8Array([commands[keysDownString]]).buffer;
    ws.send(buffer);
  }
}
