const canvas = document.getElementById('canvas')
const ctx = canvas.getContext('2d')
const width = 800
const height = 800
const size = 50
const GridSize = 16
const ms = 100

let Timer = 0
let Wave = 0
let Gold = 0
let Enemies = 0
let LastClick = {} // {x: number, y: number}

function generateHeaderText(timer, gold, wave, enemies, x, y) {
    return `Timer: ${Math.floor(timer/1000)}s | Gold: ${gold} | Wave: ${wave} | Enemy Remainings: ${enemies} | x = ${x} | y = ${y}`
}

function draw() {
    drawGrid()
}

function update() { 
    const headerText = document.getElementById('headertext')
    headerText.innerText = generateHeaderText(Timer, Gold, Wave, Enemies, LastClick.x, LastClick.y)

}

function mainLoop() {
    update()
    draw()
    console.log('Update', Timer)
    Timer += ms
}

setInterval(mainLoop, ms)

function drawGrid() {
    const grid = generateGrid()

    ctx.beginPath()
    for (const x in grid) {
        for (const y in grid) {
            if (grid[y][x] == 0) {
                ctx.fillStyle = 'green'
                ctx.fillRect(x * size, y * size, size, size)
            }
            if (grid[y][x] == 1) {
                ctx.fillStyle = 'brown'
                ctx.fillRect(x * size, y * size, size, size)
            }
        }
    }
    for (let x = 0; x <= width; x += size) {
        ctx.moveTo(x, size)
        ctx.lineTo(x, height - size)
    }
    for (let y = 0; y <= height; y += size) {
        ctx.moveTo(size, y)
        ctx.lineTo(width - size, y)
    }
    ctx.stroke()
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
    if (obj.x < 0 || obj.x > MAX) return {x: undefined, y: undefined};
    if (obj.y < 0 || obj.y > MAX) return {x: undefined, y: undefined}
    return obj
}


function getMousePosition(canvas, event) {
    let rect = canvas.getBoundingClientRect();
    let x = event.clientX - rect.left;
    let y = event.clientY - rect.top;
    ctx.fillStyle = 'black'
    ctx.font = '24px Arial'
    //ctx.fillText(`x = ${x} | y = ${y}`, 5, 25)
    //ctx.fillText(headerText, 5, 25)
    const pos = mousePos(x, y)
    LastClick = pos
    //ctx.fillText(generateHeaderText(Timer, Gold, Wave, Enemies, pos.x, pos.y), 5, 25)
    //console.log("Coordinate x: " + x,
    //    "Coordinate y: " + y);
}

let canvasElem = document.querySelector("canvas");

canvasElem.addEventListener("mousedown", function (e) {
    getMousePosition(canvasElem, e);
});

mainLoop()