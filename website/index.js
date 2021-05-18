const canvas = document.getElementById('canvas')
const ctx = canvas.getContext('2d')
const width = 800
const height = 800
const size = 50

const grid = [16][16]
console.log(grid)

ctx.beginPath()
for (var x = size; x <= width - size; x += size) {
    ctx.moveTo(x, size)
    ctx.lineTo(x, height - size)
}
for (var y = size; y <= height - size; y += size) {
    ctx.moveTo(size, y)
    ctx.lineTo(width - size, y)
}
ctx.stroke()