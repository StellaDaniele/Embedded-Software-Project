import pygame
import serial
import time

ser = serial.Serial("COM4", 1000000, serial.EIGHTBITS, serial.PARITY_NONE, serial.STOPBITS_ONE, timeout=None)

pygame.init()

command = ['*', 'R', 'D', 'Y', '*']

WIDTH = 320
HEIGHT = 240

rgb = [[0 for _ in range(2 * WIDTH)] for _ in range(2 * HEIGHT)]

try:
    while True:
        valid = False
        print("Waiting for image...")
        i = 0
        
        while True:
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
        
        window = pygame.display.set_mode((WIDTH, HEIGHT))
        #pygame.display.set_mode((0, 0), pygame.FULLSCREEN)
        
        # Clear read buffer
        _ = ser.read_all()
        
        print("Reading values...")
        
        tmp = 0
        
        for y in range(HEIGHT):
            for x in range(WIDTH):
                tmp = int.from_bytes(ser.read(), byteorder="big")
                rgb[y][x] = tmp & 0xFF
                
            if y % 10 == 0:
                print(y, ser.inWaiting())
                
            if ser.inWaiting() > 0:
                time.sleep(0.01)
        
        surface = pygame.Surface((WIDTH, HEIGHT))
        
        for y in range(HEIGHT):
            for x in range(WIDTH):
                r = g = b = rgb[y][x]
                pygame.draw.rect(surface, (r, g, b), (x, y, 1, 1))
                
        window.blit(pygame.transform.scale(surface, (1920, 1080)), (0, 0))
        pygame.display.update()
        
        print("Image rendered")
        ser.read_all()
        
except (KeyboardInterrupt, SystemExit):
    raise
    
except Exception as e:
    print(e)
    pygame.event.clear()
    ser.close()
    pygame.quit()
    raise
