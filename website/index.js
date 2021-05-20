const canvas = document.getElementById('canvas')
const ctx = canvas.getContext('2d')
const Width = 800
const Height = 800
const SquareSize = 50
const GridSize = 14
const ms = 100

const Grid = generateGrid()

let Enemies = []
let Turrets = []

const GRASS = 0
const PATH = 1
const TURRET = 2

let Timer = 0
let Wave = 0
let Gold = 100
let LastClick = {} // {x: number, y: number}

let IsGameOver = false

const Spawn = {
    x: 0,
    y: 1
}

const End = {
    x: 13,
    y: 12
}

function generateHeaderText(timer, x, y) {
    return `Timer: ${Math.floor(timer / 1000)}s |  x = ${x} | y = ${y}`
}


function draw() {
    //console.log('draw()')
    ctx.fillStyle = 'green'
    ctx.fillRect(0, 0, Width, Height)
    drawGrid()
    drawLasers()
    if (IsGameOver) {
        ctx.fillStyle = 'red'
        ctx.font = '125px Arial'
        ctx.fillText('GAME  OVER', 0, 450)
    }
}

const GameTick = 5

let NextGameUpdate = GameTick

function updateEnemies() {

    function getNextAvailablePosition(enemy) {
        const x = enemy.x
        const y = enemy.y
        const excluding = enemy.excluding
        for (const mode of [0, 1]) {
            for (let i = -1; i <= 1; i++) {
                for (let j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0) continue;
                    currentX = x + i
                    currentY = y + j
                    if (mode == 0 && (i != 0 && j != 0)) continue;
                    if (Grid[currentY] == null) continue;
                    if (Grid[currentY][currentX] == null) continue;
                    currentValue = Grid[currentY][currentX]
                    if (currentValue == PATH) {
                        if (excluding.some(pos => pos.x == currentX && pos.y == currentY)) {
                            continue;
                        }
                        return { x: currentX, y: currentY }
                    }
                }
            }
        }
        console.log('Invalid getNextAvailablePosition, ', x, y)
        return null
    }

    for (const e of Enemies) {
        console.log(e)
        const pos = getNextAvailablePosition(e)
        if (pos == null) {
            gameOver()
        }
        e.excluding.push({
            x: e.x,
            y: e.y
        })
        e.x = pos.x
        e.y = pos.y
    }
}

let Lasers = []

function removeEnemy(enemy) {
    console.log(`removing enemy ${enemy}`)
    Enemies = Enemies.filter(x => x != enemy)
}

function updateTurrets() {
    Lasers = []
    for (const turret of Turrets) {
        //console.log(turret)
        const x = turret.x
        const y = turret.y
        for (let i = -1; i <= 1; i++) {
            for (let j = -1; j <= 1; j++) {
                currentX = x + i
                currentY = y + j
                if (Enemies.some(e => e.x == currentX && e.y == currentY)) {
                    console.log(`Enemy found on ${currentX}/${currentY} by ${x}/${y}`)
                    Lasers.push({
                        fromX: x,
                        fromY: y,
                        toX: currentX,
                        toY: currentY
                    })
                    const enemy = Enemies.find(e => e.x == currentX && e.y == currentY)
                    if (!enemy) return;
                    enemy.hp -= 1
                    if (enemy.hp <= 0) {
                        enemy.dead = true
                        removeEnemy(enemy)
                    }
                    
                }
            }
        }
    }
}

function update() {
    const headerText = document.getElementById('headertext')
    headerText.innerText = generateHeaderText(Timer, LastClick.x, LastClick.y)

    const gold = document.getElementById("gold")
    gold.innerText = Gold

    updateEnemies()
    updateTurrets()
}

function mainLoop() {
    if (NextGameUpdate > 0) {
        NextGameUpdate--
        return;
    }
    else {
        NextGameUpdate = GameTick
    }


    update()
    draw()
    Timer += ms
}

setInterval(mainLoop, ms)

function gridCoordsToCanvasCoords(x, y) {
    return {
        x: Math.floor((x + 1.5) * SquareSize),
        y: Math.floor((y + 1.5) * SquareSize),
    }
}

function drawLasers() {
    for (const laser of Lasers) {
        const fromPos = gridCoordsToCanvasCoords(laser.fromX, laser.fromY)
        const toPos = gridCoordsToCanvasCoords(laser.toX, laser.toY)
        //console.log(fromPos, toPos)
        ctx.strokeStyle = 'red'
        ctx.lineWidth = 3
        ctx.beginPath()
        ctx.moveTo(fromPos.x, fromPos.y)
        ctx.lineTo(toPos.x, toPos.y)
        ctx.stroke()
        ctx.lineWidth = 1
    }
}

function drawGrid() {
    ctx.beginPath()
    const offset = SquareSize
    let tri = new Image();   // Create new img element
    tri.src = './Pictures/Towers/greyTriangle.png'; // Set source path
    for (const x in Grid) {
        for (const y in Grid) {
            if (Grid[y][x] == GRASS) {
                ctx.fillStyle = 'green'
                ctx.fillRect(offset + x * SquareSize, offset + y * SquareSize, SquareSize, SquareSize)
            }
            if (Grid[y][x] == PATH) {
                ctx.fillStyle = '#e59400'
                ctx.fillRect(offset + x * SquareSize, offset + y * SquareSize, SquareSize, SquareSize)
            }
            if (Grid[y][x] == TURRET) {
                ctx.fillStyle = 'silver'
                ctx.drawImage(tri, offset + x * SquareSize, offset + y * SquareSize, SquareSize, SquareSize)
            }
        }
    }

    ctx.strokeStyle = "rgba(100, 100, 100, 0.5"
    for (let x = 0; x <= Width; x += SquareSize) {
        ctx.moveTo(x, SquareSize)
        ctx.lineTo(x, Height - SquareSize)
        ctx.stroke()
    }
    for (let y = 0; y <= Height; y += SquareSize) {
        ctx.moveTo(SquareSize, y)
        ctx.lineTo(Width - SquareSize, y)
        ctx.stroke()
    }
    for (const x in Grid) {
        for (const y in Grid) {
            if (Grid[y][x] == TURRET) {
                ctx.strokeStyle = 'red'
                ctx.beginPath();
                ctx.moveTo(x * SquareSize, y * SquareSize)
                ctx.lineTo(3 * offset + x * SquareSize, y * SquareSize)
                ctx.lineTo(3 * offset + x * SquareSize, 3 * offset + y * SquareSize)
                ctx.lineTo(x * SquareSize, 3 * offset + y * SquareSize)
                ctx.lineTo(x * SquareSize, y * SquareSize)
                ctx.stroke()
            }
        }
    }

    let cir = new Image();   // Create new img element
    cir.src = './Pictures/Enemies/orange_circle.png'; // Set source path

    for (const e of Enemies) {
        //console.log(e)
        ctx.drawImage(cir, offset + e.x * SquareSize, offset + e.y * SquareSize, SquareSize, SquareSize)
    }
}

function generateGrid() {
    let list = []
    for (let i = 0; i < GridSize; i++) {
        let sublist = []
        for (let j = 0; j < GridSize; j++) {
            sublist.push(0)
        }
        list.push(sublist)
    }
    return list
}


function mousePos(x, y) {
    let obj = {
        x: Math.floor(x / SquareSize) - 1,
        y: Math.floor(y / SquareSize) - 1
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
    ctx.font = 'bold 24px Arial'
    const pos = mousePos(x, y)
    if (!pos.x == undefined || pos.y == undefined) return;
    LastClick = pos
    //if (GridPos[pos.y][pos.x] == 0 && LastClick)
    if (Gold <= 0) return;
    if (Grid[pos.y][pos.x] == 0 && LastClick) {
        Grid[pos.y][pos.x] = 2
        Gold -= 10
        Turrets.push({
            x: pos.x,
            y: pos.y
        })
    }
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

const EnemiesJson = {
    normal: {
        health: 5
    },
    tank: {
        health: 10
    },
    weak: {
        health: 3
    }
}

const Selected = "grey"
CurrentIdNumber = 0
function generateId() {
    CurrentIdNumber++
    return CurrentIdNumber;
}

function spawnEnemy(name) {
    const hp = EnemiesJson[name].health
    if (!hp) return;
    Enemies.push({
        id: generateId(),
        x: Spawn.x,
        y: Spawn.y,
        hp: 3,
        max: 3,
        excluding: [],
        dead: false
    })
}

let IsWaveStarted = false

function pressStartWave() {
    if (IsWaveStarted)
        return
    IsWaveStarted = true
    IsGameOver = false
    spawnEnemy('normal')
    console.log('start wave')
    const btn = document.getElementById('startwave')
    btn.classList.add('waveactive')
    btn.classList.remove('waveinactive')
}


function stopWave() {
    const btn = document.getElementById('startwave')
    btn.classList.add('waveinactive')
    btn.classList.remove('waveactive')
    IsWaveStarted = false
}

function gameOver() {
    stopWave()
    Enemies = []
    Lasers = []
    Turrets = []
    IsGameOver = true
}