"""This example lights up the NeoPixels with a rainbow swirl."""

import time
from rainbowio import colorwheel
import array
import math
import board
import audiobusio
from adafruit_circuitplayground import cp

def constrain(value, floor, ceiling):
    return max(floor, min(value, ceiling))


def log_scale(input_value, input_min, input_max, output_min, output_max):
    normalized_input_value = (input_value - input_min) / (input_max - input_min)
    return output_min + math.pow(normalized_input_value, 0.630957) * (
        output_max - output_min
    )


def normalized_rms(values):
    minbuf = int(sum(values) / len(values))
    return math.sqrt(
        sum(float(sample - minbuf) * (sample - minbuf) for sample in values)
        / len(values)
    )


def rainbow_cycle(wait):

    for j in range(255):

        for i in range(cp.pixels.n):

            idx = int((i * 256 / len(cp.pixels)) + j)

            cp.pixels[i] = colorwheel(idx & 255)

        cp.pixels.show()

        time.sleep(wait)



cp.pixels.auto_write = False

cp.pixels.brightness = 0.3

mic = audiobusio.PDMIn(
    board.MICROPHONE_CLOCK, board.MICROPHONE_DATA, sample_rate=16000, bit_depth=16
)

samples = array.array("H", [0] * 160)
mic.record(samples, len(samples))
input_floor = normalized_rms(samples) + 10

# Lower number means more sensitive - more LEDs will light up with less sound.
sensitivity = 500
input_ceiling = input_floor + sensitivity

peak = 0

rainbow_cycle(0.001)  # rainbowcycle with 1ms delay per step

time.sleep(2)

while True:
    mic.record(samples, len(samples))
    magnitude = normalized_rms(samples)
    print((magnitude,))

    c = log_scale(
        constrain(magnitude, input_floor, input_ceiling),
        input_floor,
        input_ceiling,
        0,
        10,
    )

    cp.pixels.fill((0, 0, 0))
    for i in range(10):
        if i < c:
            cp.pixels[i] = (i * (255 // 10), 50, 0)
        if c >= peak:
            peak = min(c, 10 - 1)
        elif peak > 0:
            peak = peak - 1
        if peak > 0:
            cp.pixels[int(peak)] = (80, 0, 255)
    cp.pixels.show()
