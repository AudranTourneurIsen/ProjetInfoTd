const express = require('express')
require('dotenv').config()
const app = express()
const PORT = process.env.PORT || 3000

const util = require('util');
const exec = util.promisify(require('child_process').exec);



app.use(express.static('website'))

app.get('/', (req, res) => {
    res.send('Hello World!!')
})

const DefaultWaves = [
    "20,w,w,t",
    "20,f,i,w",
    "30,t,w,w,w,w,w,w,t",
    "30,n,i,n,w,f",
    "40,n,w,w,w,i,i",
    "40,w,i,i,i,t,n",
    "50,w,w,w,n,t,i,w,f,t",
    "50,i,n,w,n,t,f,f,w,w,t,w"
]

const GridSize = 14

function gridToString(str) {
    result = ""
    //console.log(str)
    const lines = str.split('\n')
    for (const line of lines) {
        for (ch of line) {
            result += ch == '@' ? 1 : 0
        }
    }
    //console.log(result)
    return result
}

const SolverCache = new Map()

let CachedLevel = null

async function getLevel() {
    if (CachedLevel)
        return CachedLevel
    try {
        const levelResult = await exec('./leveldesigner.exe')
        const grid = gridToString(levelResult.stdout)
        let successCount = 0
        for (const waveStr of DefaultWaves) {
            const gold = Number(waveStr.split(',')[0])
            const enemiesArray = waveStr.split(',')
            enemiesArray.shift()
            enemiesStr = enemiesArray.join(',')
            const cmd = `./solver.exe solve ${grid} ${gold} ${enemiesStr}`
            //console.log(cmd)
            const solverResult = await exec(cmd)
            if (solverResult.stdout.includes('successful')) {
                const key = `${grid}-${gold}-${enemiesStr}`
                SolverCache.set(key, solverResult.stdout)
                //console.log(`Saving ${key}`)
                successCount++;
            }
            else {
                //console.log('No solution found for this level, regenerating a new one')
                return getLevel()
            }
        }
        if (successCount == DefaultWaves.length) {
            let index = 0
            console.log('Successfully generated random level')

            if (CachedLevel)
                return CachedLevel

            CachedLevel = levelResult.stdout
            return levelResult.stdout
        }
        else {
            //console.log('No solution found for this level, regenerating a new one')
            return getLevel()
        }
    } catch(e) {
        console.log('Regenerating a new level')
        getLevel()
    }
}


app.get('/api/leveldesigner', async (req, res) => {
    console.log('CachedLevel', CachedLevel)
    if (CachedLevel)
        res.send(CachedLevel)
    else
        res.send(await getLevel())
})

app.get('/api/solver', (req, res) => {
    //console.log(SolverCache)
    const grid = req.query.grid
    const gold = req.query.gold
    const wave = req.query.wave
    const cacheKey = `${grid}-${gold}-${wave}`
    if (SolverCache.has(cacheKey)) {
        console.log('Cached!')
        res.send(SolverCache.get(cacheKey))
        return
    }
    else {
        console.log('Not cached')
    }
    const { exec } = require('child_process');
    console.log(req.query)
    exec(`./solver.exe solve ${grid} ${gold} ${wave}`, (err, stdout, stderr) => {
        if (err) {
            res.send('Fatal error')
            console.error(err)
        } else {
            SolverCache.set(cacheKey, stdout)
            res.send(stdout)
        }
        return
    })
})

app.get('/api/getwaves', async (req, res) => {
    res.send(DefaultWaves.join('\n'))
})


app.get('/api/test', async (req, res) => {
    console.log(CachedLevel)
    console.log(SolverCache)
    res.send('test')
})

app.listen(PORT, () => {
    console.log(`Successfully started app PolygonTD, listening at http://localhost:${PORT}`)
    console.log('Please wait a few seconds while the first level is generated and solved')
})

getLevel()