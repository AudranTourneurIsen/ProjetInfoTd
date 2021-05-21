const canvas = document.getElementById('canvas')
const ctx = canvas.getContext('2d')
const Width = 800
const Height = 800
const SquareSize = 50
const GridSize = 14
const GameTickMs = 250
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
let Wave = 1

const Levels = {
    1: {
        enemies: ["weak", "normal", "tank"],
        gold: 25
    },
    2: {
        enemies: ["weak", "weak", "normal", "normal", "tank"],
        gold: 30
    },
    3: {
        enemies: ["tank", "weak", "weak", "normal", "normal", "weak", "weak", "tank"],
        gold: 30
    }
}

let MaxWave = Object.keys(Levels).length
let Gold = 20
let LastClick = {} // {x: number, y: number}

let IsGameOver = false
let IsWaveClear = false
let ShowWaveClear = 0

const Spawn = {
    x: -1,
    y: 1
}

const End = {
    x: 13,
    y: 12
}

let RemainingEnemiesToSpawn = []

let EnemiesCounts = {
    tank: 0,
    weak: 0,
    normal: 0
}


function resetEnemiesCount() {
    EnemiesCounts = {
        tank: 0,
        weak: 0,
        normal: 0
    }
}

let IsWaveStarted = false

function generateHeaderText(timer, x, y) {
    return `Timer: ${Math.floor(timer / 1000)}s |  x = ${x} | y = ${y}`
}

let ShowGameOver = 0

function draw() {
    ctx.fillStyle = 'green'
    ctx.fillRect(0, 0, Width, Height)
    drawGrid()
    drawLasers()
    drawEnemies()
    if (IsGameOver && ShowGameOver > 0) {
        ctx.fillStyle = 'red'
        ctx.font = '125px Arial'
        ctx.fillText('GAME  OVER', 0, 450)
        ShowGameOver--
    }
    if (IsWaveClear && ShowWaveClear > 0) {
        ctx.fillStyle = 'black'
        ctx.font = '100px Arial'
        ctx.fillText('WAVE  CLEAR', 65, 450)
        ShowWaveClear--
    }
}


//let NextGameUpdate = GameTick

function updateEnemies() {
    if (!IsWaveStarted) return;

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
        return null
    }

    for (const e of Enemies) {
        if (e.x == null || e.y == null) continue;
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
    EnemiesCounts[enemy.type]--
    enemy.htmlElement.remove()
}

function updateTurrets() {
    Lasers = []
    for (const turret of Turrets) {
        const x = turret.x
        const y = turret.y
        const enemiesInRange = []
        for (let i = -1; i <= 1; i++) {
            for (let j = -1; j <= 1; j++) {
                currentX = x + i
                currentY = y + j
                if (Enemies.some(e => e.x == currentX && e.y == currentY && e.dead == false)) {
                    console.log(`Enemy found on ${currentX}/${currentY} by ${x}/${y}`)
                    const enemy = Enemies.find(e => e.x == currentX && e.y == currentY && e.dead == false)
                    enemiesInRange.push(enemy)
                }
            }
        }
        if (enemiesInRange.length == 0) continue;
        if (turret.cooldown > 0) {
            turret.cooldown--
            continue
        }
        turret.cooldown = TurretsJson[turret.type].attack_speed - 1
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
        const dmg = TurretsJson[turret.type].damage
        minEnemy.hp -= dmg
        const progress = minEnemy.htmlElement.children[1]
        progress.value -= dmg
        if (minEnemy.hp <= 0) {
            minEnemy.dead = true
            //removeEnemy(minEnemy)
        }
    }
}

let spawnSpace = false

let CurrentTick = 0

function update() {
    CurrentTick++
    for (const e of Enemies) {
        if (e.dead) {
            removeEnemy(e)
        }
    }
    const headerText = document.getElementById('headertext')
    headerText.innerText = generateHeaderText(Timer, LastClick.x, LastClick.y)

    const gold = document.getElementById("gold")
    gold.innerText = Gold

    const waveCounter = document.getElementById('WaveCounter')
    waveCounter.innerText = `${Wave} / ${MaxWave}`

    const normal = document.getElementById("NormalEnemyNumber")
    const weak = document.getElementById("WeakEnemyNumber")
    const tank = document.getElementById("TankEnemyNumber")

    if (normal) normal.innerText = `x ${EnemiesCounts.normal}`
    if (weak) weak.innerText = `x ${EnemiesCounts.weak}`
    if (tank) tank.innerText = `x ${EnemiesCounts.tank}`

    const remaining = document.getElementById('EnemyRemaining')
    if (remaining) remaining.innerText = 'Remaining : ' + (EnemiesCounts.normal + EnemiesCounts.tank + EnemiesCounts.weak)

    if (!IsWaveStarted) return

    for (const e of Enemies.filter(x => !x.ready)) {
        //spawnSpace = !spawnSpace
        //if (spawnSpace) {
        if (CurrentTick % 2 == 0) {
            console.log(CurrentTick, 'spawning')
            spawnEnemy(e)
            break
        }
    }

    updateEnemies()
    updateTurrets()

    if (Enemies.length == 0) {
        waveClear()
    }
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
            if (Grid[y][x] == TURRET_CLASSIC || Grid[y][x] == TURRET_FAST || Grid[y][x] == TURRET_HEAVY) {
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
}


function drawEnemies() {
    if (!IsWaveStarted) return
    const offset = SquareSize
    for (const e of Enemies) {
        if (e.x == null || e.y == null) continue;
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
    if (!SelectedTurret) return;
    function getPricePerTurret(name) {
        if (name == 'heavy') return 15
        if (name == 'fast') return 10
        if (name == 'classic') return 5
    }

    function idToName(id) {
        if (id == TURRET_CLASSIC) return 'classic'
        if (id == TURRET_HEAVY) return 'heavy'
        if (id == TURRET_FAST) return 'fast'
    }

    if (SelectedTurret != 'sell') {
        const price = getPricePerTurret(SelectedTurret)
        if ((Gold - price) < 0) return;
        Gold -= price
        if (Grid[pos.y][pos.x] == GRASS && LastClick) {
            Grid[pos.y][pos.x] = TurretsJson[SelectedTurret].id

            Turrets.push({
                x: pos.x,
                y: pos.y,
                type: SelectedTurret,
                cooldown: 0
            })
        }
    }
    else {
        if (SelectedTurret == 'sell') {
            if ([TURRET_CLASSIC, TURRET_FAST, TURRET_HEAVY].includes(Grid[pos.y][pos.x])) {
                const id = Grid[pos.y][pos.x] 
                console.log(id, idToName(id), getPricePerTurret(idToName(id)))
                const price = getPricePerTurret(idToName(id))
                Grid[pos.y][pos.x] = GRASS
                Gold += price
                Turrets = Turrets.filter(t => t.x != pos.x && t.y != pos.y)
            }
        }
    }
}

let canvasElem = document.querySelector("canvas");

canvasElem.addEventListener("mousedown", function (e) {
    getMousePosition(canvasElem, e);
});

mainLoop()

let GlobalTxt = null

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
                GlobalTxt = txt
                resetGrid(txt)
            }
        }
    }
}

importGridFromText()

function resetGrid(txt) {
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

const EnemiesJson = {
    weak: {
        health: 3,
        img: "lime_circle.png"
    },
    normal: {
        health: 5,
        img: "orange_circle.png"
    },
    tank: {
        health: 15,
        img: "purple_circle.png"
    }
}

const TurretsJson = {
    classic: {
        attack_speed: 2,
        damage: 1,
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
        attack_speed: 15,
        damage: 15,
        img: "red_Triangle.png",
        id: TURRET_HEAVY,
    },
}

let SelectedTurret = null
CurrentIdNumber = 0
function generateId() {
    CurrentIdNumber++
    return CurrentIdNumber;
}

function enemyToImg(name) {
    if (name == 'weak') return 'Pictures/Enemies/lime_circle.png'
    if (name == 'normal') return 'Pictures/Enemies/orange_circle.png'
    if (name == 'tank') return 'Pictures/Enemies/purple_circle.png'
}

function spawnEnemy(enemy) {
    enemy.x = Spawn.x
    enemy.y = Spawn.y
    enemy.ready = true
}

function instanciateEnemy(name) {
    console.log(name)
    const hp = EnemiesJson[name].health
    if (!hp) return;
    const htmlElem = cloneHtmlEnemyElement()
    const img = htmlElem.firstElementChild
    const progress = htmlElem.children[1]
    progress.max = hp
    progress.value = hp
    console.log(progress)

    img.src = enemyToImg(name)
    Enemies.push({
        id: generateId(),
        type: name,
        //x: Spawn.x,
        //y: Spawn.y,
        x: null,
        y: null,
        hp: hp,
        max: hp,
        excluding: [],
        dead: false,
        htmlElement: htmlElem,
        ready: false
    })
}

function instanciateEnemies() {
    //RemainingEnemiesToSpawn = enemiesToSpawn.map(x => x) 
    Turrets = []

    const remaining = Levels[Wave].enemies.map(x => x)
    Gold = Levels[Wave].gold
    for (const type of remaining) {
        EnemiesCounts[type]++
        instanciateEnemy(type)
    }
}

function pressStartWave() {
    if (IsWaveStarted)
        return
    IsWaveStarted = true
    IsGameOver = false
    ShowGameOver = 10
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

function waveClear() {
    IsWaveClear = true
    ShowWaveClear = 10
    IsWaveStarted = false
    const btn = document.getElementById('startwave')
    btn.classList.add('waveinactive')
    btn.classList.remove('waveactive')
    const waveCounter = document.getElementById('WaveCounter')
    Wave++
    waveCounter.innerText = `${Wave} / ${MaxWave}`

    Turrets = []

    resetGrid(GlobalTxt)
    resetEnemiesCount()
    instanciateEnemies()
}

function gameOver() {
    stopWave()
    Lasers = []
    IsGameOver = true
    for (const enemy of Enemies) {
        enemy.htmlElement.remove()
    }
    Enemies = []
    for (const turret of Turrets) {
        turret.cooldown = 0
    }

    Turrets = []
    resetGrid(GlobalTxt)
    resetEnemiesCount()
    instanciateEnemies()
}

function generateMobHud() {
    const alive = document.getElementById('EnemyAlive')
}

function unselectAll() {
    for (const id of ['Sell', 'ClassicTurret', 'FastTurret', 'HeavyTurret']) {
        const elem = document.getElementById(id)
        elem.classList.remove('Selected')
    }
}

function manageTurretSelection() {
    const classic = document.getElementById('ClassicTurret')
    classic.addEventListener('mousedown', () => {
        SelectedTurret = 'classic'
        unselectAll()
        classic.classList.add('Selected')
    })

    const fast = document.getElementById('FastTurret')
    fast.addEventListener('mousedown', () => {
        SelectedTurret = 'fast'
        unselectAll()
        fast.classList.add('Selected')
    })

    const heavy = document.getElementById('HeavyTurret')
    heavy.addEventListener('mousedown', () => {
        SelectedTurret = 'heavy'
        unselectAll()
        heavy.classList.add('Selected')
    })

    const cancel = document.getElementById('Cancel')
    cancel.addEventListener('mousedown', () => {
        SelectedTurret = null
        unselectAll()
    })

    const sell = document.getElementById('Sell')
    sell.addEventListener('mousedown', () => {
        SelectedTurret = 'sell'
        unselectAll()
        sell.classList.add('Selected')
    })
}

manageTurretSelection()

function cloneHtmlEnemyElement() {
    const ref = document.getElementById('MobReference')
    const parent = ref.parentElement
    const cloned = ref.cloneNode(true)
    cloned.classList.remove('disabled')
    parent.append(cloned)
    return cloned
}

instanciateEnemies()

function setWave(x) {
    Wave = x
    Lasers = []
    for (const enemy of Enemies) {
        enemy.htmlElement.remove()
    }
    Enemies = []
    Turrets = []
    resetGrid(GlobalTxt)
    resetEnemiesCount()
    instanciateEnemies()
}