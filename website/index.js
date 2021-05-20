const canvas = document.getElementById('canvas')
const ctx = canvas.getContext('2d')
const Width = 800
const Height = 800
const SquareSize = 50
const GridSize = 14
const GameTickMs = 500
//const GameTick = 5

const Grid = generateGrid()

let Enemies = []
let Turrets = []

const GRASS = 0
const PATH = 1
const TURRET_CLASSIC = 2
const TURRET_FAST = 3
const TURRET_HEAVY = 4

let Timer = 0
let Wave = 0
let Gold = 100
let LastClick = {} // {x: number, y: number}

let IsGameOver = false

const Spawn = {
    x: -1,
    y: 1
}

const End = {
    x: 13,
    y: 12
}

let RemainingEnemiesToSpawn = []

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


//let NextGameUpdate = GameTick

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
        const enemiesInRange = []
        for (let i = -1; i <= 1; i++) {
            for (let j = -1; j <= 1; j++) {
                currentX = x + i
                currentY = y + j
                if (Enemies.some(e => e.x == currentX && e.y == currentY)) {
                    console.log(`Enemy found on ${currentX}/${currentY} by ${x}/${y}`)
                    const enemy = Enemies.find(e => e.x == currentX && e.y == currentY)
                    enemiesInRange.push(enemy)


                }
            }
        }
        if (enemiesInRange.length == 0) continue;
        let minEnemy = enemiesInRange[0]
        for (const enemy of enemiesInRange) {
            if (enemy.id < minEnemy.id) {
                minEnemy = enemy
            }
        }
        Lasers.push({
            fromX: turret.x,
            fromY: turret.y,
            toX: minEnemy.x,
            toY: minEnemy.y
        })
        minEnemy.hp -= 1
        if (minEnemy.hp <= 0) {
            minEnemy.dead = true
            removeEnemy(minEnemy)
        }
    }
}

let spawnSpace = false

function update() {
    const headerText = document.getElementById('headertext')
    headerText.innerText = generateHeaderText(Timer, LastClick.x, LastClick.y)

    const gold = document.getElementById("gold")
    gold.innerText = Gold

    if (RemainingEnemiesToSpawn.length > 0) {
        spawnSpace = !spawnSpace
        if (spawnSpace) {
            spawnEnemy(RemainingEnemiesToSpawn[0])
            RemainingEnemiesToSpawn.shift()
        }
    }

    updateEnemies()
    updateTurrets()
}

function mainLoop() {
    /*
    if (NextGameUpdate > 0) {
        NextGameUpdate--
        return;
    }
    else {
        NextGameUpdate = GameTick
    }
    */


    update()
    draw()
    Timer += GameTickMs
}

setInterval(mainLoop, GameTickMs)

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
            if (Grid[y][x] == TURRET_CLASSIC) {
                let tri = new Image();   // Create new img element
                tri.src = './Pictures/Towers/greyTriangle.png'; // Set source path
                ctx.drawImage(tri, offset + x * SquareSize, offset + y * SquareSize, SquareSize, SquareSize)
            }
            if (Grid[y][x] == TURRET_FAST) {
                tri = new Image();   // Create new img element
                tri.src = './Pictures/Towers/yellowTriangle.png'; // Set source path
                ctx.drawImage(tri, offset + x * SquareSize, offset + y * SquareSize, SquareSize, SquareSize)
            }
            if (Grid[y][x] == TURRET_HEAVY) {
                tri = new Image();   // Create new img element
                tri.src = './Pictures/Towers/redTriangle.png'; // Set source path
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
            if (Grid[y][x] == TURRET_CLASSIC || Grid[y][x] == TURRET_FAST || Grid[y][x] ==TURRET_HEAVY) {
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


    for (const e of Enemies) {
        let cir = new Image();   // Create new img element
        //cir.src = './Pictures/Enemies/orange_circle.png'; // Set source path
        cir.src = `./Pictures/Enemies/${EnemiesJson[e.type].img}`
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
        Grid[pos.y][pos.x] = TurretsJson[SelectedTurret].id
        Gold -= 10
        Turrets.push({
            x: pos.x,
            y: pos.y,
            type: SelectedTurret
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
        health: 5,
        img: "orange_circle.png"
    },
    tank: {
        health: 10,
        img: "red_circle.png"
    },
    weak: {
        health: 3,
        img: "lime_circle.png"
    }
}

const TurretsJson = {
    classic: {
        attack_speed: 2,
        damage: 2,
        img: "grey_Triangle.png",
        id: TURRET_CLASSIC,
    },

    fast: {
        attack_speed: 1,
        damage: 1,
        img: "yellow_Triangle.png",
        id: TURRET_FAST,
    },

    heavy: {
        attack_speed: 9,
        damage: 10,
        img: "red_Triangle.png",
        id: TURRET_HEAVY,
    },
}

let SelectedTurret = "classic"
CurrentIdNumber = 0
function generateId() {
    CurrentIdNumber++
    return CurrentIdNumber;
}

function spawnEnemy(name) {
    console.log(name)
    const hp = EnemiesJson[name].health
    if (!hp) return;
    Enemies.push({
        id: generateId(),
        type: name,
        x: Spawn.x,
        y: Spawn.y,
        hp: EnemiesJson[name].health,
        max: EnemiesJson[name].health,
        excluding: [],
        dead: false
    })
}

let IsWaveStarted = false

const enemiesToSpawn = [
    "normal", "normal", "weak", "tank"
]


async function spawnEnemies() {
    RemainingEnemiesToSpawn = enemiesToSpawn.map(x => x)
}

function pressStartWave() {
    if (IsWaveStarted)
        return
    IsWaveStarted = true
    IsGameOver = false
    //spawnEnemy('normal')
    spawnEnemies()
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
    IsGameOver = true
}

function generateMobHud() {
    const alive = document.getElementById('EnemyAlive')
}

function manageTurretSelection() {
    const classic = document.getElementById('ClassicTurret')
    classic.addEventListener('mousedown', () => {
        console.log('classic')
        SelectedTurret = 'classic'
    })

    const fast = document.getElementById('FastTurret')
    fast.addEventListener('mousedown', () => {
        console.log('fast')
        SelectedTurret = 'fast'
    })

    const heavy = document.getElementById('HeavyTurret')
    heavy.addEventListener('mousedown', () => {
        console.log('heavy')
        SelectedTurret = 'heavy'
    })

}

manageTurretSelection()