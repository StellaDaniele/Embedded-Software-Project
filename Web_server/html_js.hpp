#ifndef __HTML_JS_HPP__
#define __HTML_JS_HPP__
/*
This file contains the HTML code that is sent by the server
to the clients. It was too messy to have everything in the main
file.
*/
const char* html = R"===(
<!DOCTYPE html>
<html>

<head>
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">
    <title>IOT_webserver</title>
    <script>
        const socket = new WebSocket('ws://192.168.12.3:81')

        socket.binaryType = 'arraybuffer'

        socket.onopen = function (event) {
            console.log('WebSocket connected')
        }

        socket.onmessage = function (event) {
            // The data received from the WebSocket is stored in the "data" property of the event object
            const brightness = new Uint8Array(event.data)

            // Use the data received
            console.log(brightness)
        }

        socket.onclose = function (event) {
            console.log('WebSocket closed')
        }

    </script>
</head>

<body>
    <h1>HOMEPAGE</h1>
</body>

</html>
)===";

#endif