const canvas = document.getElementById('canvas')
const ctx = canvas.getContext('2d')
const width = 800
const height = 800
const size = 50
const GridSize = 16

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

drawGrid()

function getMousePosition(canvas, event) {
    let rect = canvas.getBoundingClientRect();
    let x = event.clientX - rect.left;
    let y = event.clientY - rect.top;
    ctx.fillStyle = 'black'
    ctx.font = '24px Arial'
    ctx.fillText(`x = ${x} | y = ${y}`, 5, 25)
    console.log("Coordinate x: " + x,
        "Coordinate y: " + y);
}

let canvasElem = document.querySelector("canvas");

canvasElem.addEventListener("mousedown", function (e) {
    getMousePosition(canvasElem, e);
});