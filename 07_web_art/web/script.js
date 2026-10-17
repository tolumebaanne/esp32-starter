// Button Art. The board sends "press:1", "press:2" or "press:3" over a live connection.
//   1 press:  a bloom of shapes opens somewhere on the screen
//   2 presses: the colours change, and the board's light changes to match
//   3 presses: a wave sweeps the screen clean
// Keys 1, 2 and 3 do the same, so the page can be tried without the board.

const canvas = document.getElementById("art");
const pen = canvas.getContext("2d");
const dot = document.getElementById("dot");
const statusText = document.getElementById("status");
const addressBox = document.getElementById("address");
const connectButton = document.getElementById("connect");
const lastText = document.getElementById("last");

// Each palette has a background, its colours, and the colour the board's light shows.
// The light shows plain red, green or blue, and the colour changes on every new palette.
const PALETTES = [
  { name: "Meadow", back: "#08130c", colours: ["#7ee081", "#c3f73a", "#2bd9a5", "#f7f7c6"], light: [0, 1, 0] },
  { name: "Ember",  back: "#160805", colours: ["#ff5a36", "#ffb347", "#ff2e63", "#ffe1a8"], light: [1, 0, 0] },
  { name: "Lagoon", back: "#04111a", colours: ["#00c2d1", "#1ee3cf", "#3a86ff", "#c9f8ff"], light: [0, 0, 1] },
  { name: "Dusk",   back: "#110717", colours: ["#c77dff", "#ff70a6", "#7b2cbf", "#ffd6ff"], light: [1, 0, 0] },
  { name: "Citrus", back: "#141205", colours: ["#ffd60a", "#ffc300", "#a7c957", "#fff3b0"], light: [0, 1, 0] },
  { name: "Ice",    back: "#050a18", colours: ["#4cc9f0", "#4361ee", "#bde0fe", "#ffffff"], light: [0, 0, 1] }
];

let palette = 0;
let shapes = [];
let sweep = null;      // the wave that clears the screen, while it is moving
let link = null;       // the live connection to the board
let retryTimer = null;

function resize() {
  canvas.width = window.innerWidth * devicePixelRatio;
  canvas.height = window.innerHeight * devicePixelRatio;
  pen.setTransform(devicePixelRatio, 0, 0, devicePixelRatio, 0, 0);
}

function pick(list) {
  return list[Math.floor(Math.random() * list.length)];
}

// 1 press: a ring of shapes bursts out from one point.
function bloom() {
  const w = window.innerWidth;
  const h = window.innerHeight;
  const x = w * (0.15 + Math.random() * 0.7);
  const y = h * (0.15 + Math.random() * 0.7);
  const count = 70;
  for (let i = 0; i < count; i = i + 1) {
    const angle = (i / count) * Math.PI * 2;
    const speed = 1.5 + Math.random() * 4;
    shapes.push({
      x: x,
      y: y,
      dx: Math.cos(angle) * speed,
      dy: Math.sin(angle) * speed,
      size: 3 + Math.random() * 9,
      colour: Math.floor(Math.random() * 4),
      life: 1,
      round: Math.random() < 0.7
    });
  }
}

// 2 presses: move to the next palette, and tell the board which colour to show.
function newColours() {
  palette = (palette + 1) % PALETTES.length;
  sendLightColour();
}

// 3 presses: a wave sweeps across and clears everything behind it.
function sweepClean() {
  sweep = { x: 0 };
}

function act(count) {
  if (count === 1) {
    bloom();
    flash("1 press: bloom");
  } else if (count === 2) {
    newColours();
    flash("2 presses: " + PALETTES[palette].name);
  } else if (count === 3) {
    sweepClean();
    flash("3 presses: sweep");
  }
}

function flash(words) {
  lastText.textContent = words;
  lastText.classList.add("show");
  setTimeout(() => lastText.classList.remove("show"), 700);
}

function draw() {
  const w = window.innerWidth;
  const h = window.innerHeight;
  const p = PALETTES[palette];

  // a see-through layer of background each frame leaves soft trails behind the shapes
  pen.globalAlpha = 0.18;
  pen.fillStyle = p.back;
  pen.fillRect(0, 0, w, h);
  pen.globalAlpha = 1;

  for (const s of shapes) {
    s.x = s.x + s.dx;
    s.y = s.y + s.dy;
    s.dx = s.dx * 0.985;
    s.dy = s.dy * 0.985 + 0.01;
    s.life = s.life - 0.004;
    pen.globalAlpha = Math.max(s.life, 0);
    pen.fillStyle = p.colours[s.colour];
    if (s.round) {
      pen.beginPath();
      pen.arc(s.x, s.y, s.size, 0, Math.PI * 2);
      pen.fill();
    } else {
      pen.fillRect(s.x - s.size / 2, s.y - s.size / 2, s.size, s.size);
    }
  }
  pen.globalAlpha = 1;
  shapes = shapes.filter(s => s.life > 0);

  if (sweep) {
    sweep.x = sweep.x + w / 40;
    pen.fillStyle = p.back;
    pen.fillRect(0, 0, sweep.x, h);
    pen.fillStyle = p.colours[0];
    pen.fillRect(sweep.x - 6, 0, 6, h);
    shapes = shapes.filter(s => s.x > sweep.x);
    if (sweep.x > w + 10) {
      sweep = null;
    }
  }

  requestAnimationFrame(draw);
}

// ---------- the live connection to the board ----------

function sendLightColour() {
  if (link && link.readyState === WebSocket.OPEN) {
    const c = PALETTES[palette].light;
    link.send("colour:" + c.join(","));
  }
}

function showStatus(connected, words) {
  dot.classList.toggle("on", connected);
  statusText.textContent = words;
}

function connect() {
  clearTimeout(retryTimer);
  if (link) {
    link.onclose = null;
    link.close();
  }
  const address = addressBox.value.trim();
  try { localStorage.setItem("boardAddress", address); } catch (e) {}
  showStatus(false, "Connecting to " + address + "...");
  link = new WebSocket("ws://" + address + ":81");

  link.onopen = () => {
    showStatus(true, "Connected to the board");
    sendLightColour();
  };
  link.onmessage = (event) => {
    const message = String(event.data);
    if (message.startsWith("press:")) {
      act(Number(message.slice(6)));
    }
  };
  link.onclose = () => {
    showStatus(false, "Not connected. Trying again...");
    retryTimer = setTimeout(connect, 2000);
  };
}

try {
  const saved = localStorage.getItem("boardAddress");
  if (saved) addressBox.value = saved;
} catch (e) {}

connectButton.addEventListener("click", connect);
addressBox.addEventListener("keydown", (e) => { if (e.key === "Enter") connect(); });
document.addEventListener("keydown", (e) => {
  if (e.target === addressBox) return;
  if (e.key === "1" || e.key === "2" || e.key === "3") act(Number(e.key));
});

window.addEventListener("resize", resize);
resize();
pen.fillStyle = PALETTES[palette].back;
pen.fillRect(0, 0, window.innerWidth, window.innerHeight);
draw();
