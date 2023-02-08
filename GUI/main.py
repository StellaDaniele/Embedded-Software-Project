import pygame
import serial
import threading
import time

# Change "COM1" and the BAUD rate as needed
ser = serial.Serial("COM4", 1000000,
                    serial.EIGHTBITS, serial.PARITY_NONE, serial.STOPBITS_ONE,
                    timeout=None)

pygame.init()

command = ['*', 'R', 'D', 'Y', '*']

# These may be actually flipped, so width is really the height and vice versa
WIDTH = 320
HEIGHT = 240

#WIDTH = 640
#HEIGHT = 480

# Store image output
rgb = [[0 for _ in range(2 * WIDTH)] for _ in range(2 * HEIGHT)]

try:

    while True:
        # Wait until we receive *RDY*, which marks the start
        # of an image
        # -----------------------------
        valid = False
        print("Waiting for image...")
        i = 0

        while True:
            # Try to decode - our command is all UTF-8 chars
            try:
                cc = ser.read().decode("utf-8")
            except (KeyboardInterrupt, SystemExit):
                raise
            except:
                continue

            if command[i] == cc:
                i += 1
            else:
                i = 0
            if i >= len(command):
                break

        print("Image found")
        # -----------------------------

        #window = pygame.display.set_mode((2 * WIDTH, 2 * HEIGHT))
        window = pygame.display.set_mode((WIDTH, HEIGHT))
        #pygame.display.set_mode((0, 0), pygame.FULLSCREEN)
        window.fill(0)

        # Clear read buffer
        _ = ser.read_all()

        print("Reading values...")

        tmp = 0

        # Read the image - just a list of bytes, column by column
        # Each byte represents the brightness 0 - 255

        for y in range(HEIGHT):
            for x in range(WIDTH):
                # byteorder doesn't matter since we're reading 1 byte at a time
                tmp = int.from_bytes(ser.read(), byteorder="big")
                rgb[y][x] = tmp & 0xFF

            if y % 10 == 0:
                print(y, ser.inWaiting())
            if ser.inWaiting() > 0:
                time.sleep(0.01)

        # Draw the image
        for y in range(HEIGHT):
            for x in range(WIDTH):
                r = g = b = rgb[y][x]
                pygame.draw.rect(window, (r, g, b), (x, y, 1, 1))
                #pygame.draw.rect(window, (r, g, b), (x * 2, y * 2, 2, 2))
                pygame.event.clear()

        pygame.display.flip()
        pygame.display.update()

        print("Image rendered")

        pygame.display.flip()
        pygame.display.update()
        ser.read_all()  # blocking
        # data = ser.read(ser.inWaiting()) # should not block but still blocks
        # time.sleep(0.001)
# except (KeyboardInterrupt, SystemExit):
#     raise
except Exception as e:
    print(e)
    pygame.event.clear()
    ser.close()
    pygame.quit()
    raise
