## Features
1. Purring
2. Vibrating
3. [DONE] Touch detection: using IR or Ultrasonic
4. Movement on Wheels: by a remote controller
5. [DONE] Eyes Show Different Emotions
6. [DONE] RGB light on top of the cat with color changes based on signals

## Steps
1. Purring
- Activate the purring mechanism upon trigger.
2. Vibrate
- Activate the servo upon trigger.
3. Detect touching [DONE]
- Use sensors (IR or Ultrasonic) to detect touch or proximity.
- Once touch is detected, send a signal to trigger the purring feature.
4. Move on wheel
- Design and implement a wheel mechanism to allow movement.
- Sync movement with remote controller input to enable control.
- Transmit signals from the remote control to the wheel mechanism for directional control.
5. Eye emotion [DONE]
- Implement LED lights to represent different emotions.
- Transmit signals from the remote control to change eye emotion:
    + Happy: ^ ^
    + Sad: - -
    + Lovely: <3 <3
6. Top light [DONE]
- Implement an RGB light that changes colors based on signals:
    + Outdoor environment: Bright white
    + Walking environment: Bright red -> help car detect pedestrian
    + Indoor reading light: Warm yellow: 0x802000
    + Relax: Dim yellow: 0x1a0600, dim: 0.5
- Transmit signals from the remote to switch between colors or moods.

## Sources
- Ultrasonic: https://projecthub.arduino.cc/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-7cabe1
