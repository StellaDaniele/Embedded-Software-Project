const width = 320
const height = 240
let brightnessArray = new Uint8Array(width * height)
let pixels = 0
let canvas_num = 0
const socket = new WebSocket('ws://192.168.12.3:81')

socket.binaryType = 'arraybuffer'

socket.onopen = function (event) {
  console.log('WebSocket connected')
}

socket.onmessage = function (event) {
  // The data received from the WebSocket is stored in the "data" property of the event object
  const brightness = new Uint8Array(event.data)

  // console.log(brightness)
  brightnessArray.set(brightness, pixels * width * 60)
  pixels++
  // pixels * width * 60 == width * height when the whole image is received
  if (pixels == 4) {
    pixels = 0
    renderImage(canvas_num++)
  }
}

socket.onclose = function (event) {
  console.log('WebSocket closed')
}

function renderImage (canvasId) {
  var canvas = document.createElement('canvas')
  canvas.id = 'canvas' + canvasId.toString()
  canvas.width = width
  canvas.height = height
  document.body.appendChild(canvas)
  //var canvas = document.getElementById(canvasId);
  var ctx = canvas.getContext('2d')
  var imageData = ctx.createImageData(width, height)
  for (var i = 0; i < brightnessArray.length; i++) {
    var brightness = brightnessArray[i]
    var index = i * 4
    imageData.data[index] = brightness
    imageData.data[index + 1] = brightness
    imageData.data[index + 2] = brightness
    imageData.data[index + 3] = 255 // complete opacity
  }
  ctx.putImageData(imageData, 0, 0)
}
