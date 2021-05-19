const canvas = document.getElementById('canvas')
const ctx = canvas.getContext('2d')
const width = 800
const height = 800
const size = 50
const GridSize = 14
const ms = 100

const Grid = generateGrid()

const GRASS = 0
const PATH = 1
const TURRET = 2

let Timer = 0
let Wave = 0
let Gold = 10
let LastClick = {} // {x: number, y: number}

const Spawn = {
    x: 0,
    y: 1
}

const End = {
    x: 13,
    y: 12
}

function generateHeaderText(timer, x, y) {
    return `Timer: ${Math.floor(timer / 1000)}s | x = ${x} | y = ${y}`
}

function draw() {
    drawGrid()
}

function update() {
    const headerText = document.getElementById('headertext')
    headerText.innerText = generateHeaderText(Timer, LastClick.x, LastClick.y)

    const gold = document.getElementById("gold")
    gold.innerText = Gold

}

function mainLoop() {
    update()
    draw()
    Timer += ms
}

setInterval(mainLoop, ms)

function drawGrid() {
    ctx.beginPath()
    const offset = size
    let img = new Image();   // Create new img element
    img.src = './img/grayTriangle.png'; // Set source path
    for (const x in Grid) {
        for (const y in Grid) {
            if (Grid[y][x] == GRASS) {
                ctx.fillStyle = 'green'
                ctx.fillRect(offset + x * size, offset + y * size, size, size)
            }
            if (Grid[y][x] == PATH) {
                ctx.fillStyle = '#e59400'
                ctx.fillRect(offset + x * size, offset + y * size, size, size)
            }
            if (Grid[y][x] == TURRET) {
                ctx.fillStyle = 'silver'
                ctx.drawImage(img, offset + x * size, offset + y * size, size, size)
            }
        }
    }

    ctx.strokeStyle = "rgba(100, 100, 100, 0.5"
    for (let x = 0; x <= width; x += size) {
        ctx.moveTo(x, size)
        ctx.lineTo(x, height - size)
        ctx.stroke()
    }
    for (let y = 0; y <= height; y += size) {
        ctx.moveTo(size, y)
        ctx.lineTo(width - size, y)
        ctx.stroke()
    }
    for (const x in Grid) {
        for (const y in Grid) {
            if (Grid[y][x] == TURRET) {
                ctx.strokeStyle = 'red'
                ctx.beginPath();
                ctx.moveTo(x * size, y * size)
                ctx.lineTo(3 * offset + x * size, y * size)
                ctx.lineTo(3 * offset + x * size, 3 * offset + y * size)
                ctx.lineTo(x * size, 3 * offset + y * size)
                ctx.lineTo(x * size, y * size)
                ctx.stroke()
            }
        }
    }
}

function generateGrid() {
    let list = []
    for (let i = 0; i < GridSize; i++) {
        let sublist = []
        for (let j = 0; j < GridSize; j++) {
            if (i == 8)
                sublist.push(1)
            else
                sublist.push(0)
        }
        list.push(sublist)
    }
    return list
}


function mousePos(x, y) {
    let obj = {
        x: Math.floor(x / size) - 1,
        y: Math.floor(y / size) - 1
    }
    const MAX = 13
    if (obj.x < 0 || obj.x > MAX) return { x: undefined, y: undefined };
    if (obj.y < 0 || obj.y > MAX) return { x: undefined, y: undefined }
    return obj
}


function getMousePosition(canvas, event) {
    let rect = canvas.getBoundingClientRect();
    let x = event.clientX - rect.left;
    let y = event.clientY - rect.top;
    ctx.fillStyle = 'black'
    ctx.font = '24px Arial'
    const pos = mousePos(x, y)
    if (!pos.x == undefined || pos.y == undefined) return;
    LastClick = pos
    //if (GridPos[pos.y][pos.x] == 0 && LastClick)
    if (Grid[pos.y][pos.x] == 0 && LastClick)
        Grid[pos.y][pos.x] = 2
}

let canvasElem = document.querySelector("canvas");

canvasElem.addEventListener("mousedown", function (e) {
    getMousePosition(canvasElem, e);
});

mainLoop()

function importGridFromText() {
    const request = new XMLHttpRequest();
    request.open('GET', 'data/arena.txt', true);
    request.send(null);
    request.onreadystatechange = function () {
        if (request.readyState === 4 && request.status === 200) {
            var type = request.getResponseHeader('Content-Type');
            if (type.indexOf("text") !== 1) {
                const txt = request.responseText
                console.log(txt);
                const lines = txt.split('\n')
                let x = 0
                let y = 0
                for (const line of lines) {
                    x = 0
                    for (const c of line) {
                        console.log(`${x} ${y}`)
                        Grid[y][x] = c == '@' ? 1 : 0
                        x++
                    }
                    y++
                }
            }
        }
    }
}

importGridFromText()
