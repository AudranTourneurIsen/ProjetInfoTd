const express = require('express')
const app = express()
const port = 3000

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
]

const GridSize = 14

function gridToString(str) {
    result = ""
    console.log(str)
    const lines = str.split('\n')
    for (const line of lines) {
        for (ch of line) {
            result += ch == '@' ? 1 : 0
        }
    }
    console.log(result)
    return result
}

async function getLevel() {
    console.log('a')
    const levelResult = await exec('./leveldesigner.exe')
    console.log('b')
    console.log(levelResult)
    const grid = gridToString(levelResult.stdout)
    let successCount = 0
    for (const waveStr of DefaultWaves) {
        const gold = Number(waveStr.split(',')[0])
        const enemiesArray = waveStr.split(',')
        enemiesArray.shift()
        enemiesStr = enemiesArray.join(',')
        const cmd = `./solver.exe solve ${grid} ${gold} ${enemiesStr}`
        console.log(cmd)
        let index = 0
        const solverResult = await exec(cmd)
        if (solverResult.stdout.includes('successful')) {
            successCount++;
        }
    }
    if (successCount == DefaultWaves.length) {
        console.log('ok')
        return levelResult.stdout
    }
    else {
        console.log('impossibe')
        return getLevel()
    }
    //gridStr = stdout;
    //console.log(gridStr);
}

app.get('/api/leveldesigner', async (req, res) => {
    res.send(await getLevel())
})

app.get('/api/solver', (req, res) => {
    const { exec } = require('child_process');
    console.log(req.query)
    exec(`./solver.exe solve ${req.query.grid} ${req.query.gold} ${req.query.wave}`, (err, stdout, stderr) => {
        if (err) {
            res.send('Fatal error')
            console.error(err)
        } else {
            res.send(stdout)
        }
    })
})

app.get('/api/getwaves', async (req, res) => {
    res.send(DefaultWaves.join('\n'))
})

app.listen(port, () => {
    console.log(`Example app listening at http://localhost:${port}`)
})