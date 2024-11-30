## Features
1. Purring
2. Vibrating
3. Touch detection: using IR or Ultrasonic
4. Movement on Wheels: by a remote controller
5. Eyes Show Different Emotions
6. RGB light on top of the cat with color changes based on signals

## Steps
1. Purring
- Activate the purring mechanism upon trigger.
2. Vibrate
- Activate the servo upon trigger.
3. Detect touching
- Use sensors (IR or Ultrasonic) to detect touch or proximity.
- Once touch is detected, send a signal to trigger the purring feature.
4. Move on wheel
- Design and implement a wheel mechanism to allow movement.
- Sync movement with remote controller input to enable control.
- Transmit signals from the remote control to the wheel mechanism for directional control.
5. Eye emotion
- Implement LED lights to represent different emotions.
- Transmit signals from the remote control to change eye emotion:
    + Happy: ^ ^
    + Sad: - -
    + Lovely: <3 <3
6. Top light
- Implement an RGB light that changes colors based on signals:
    + Outdoor environment: Bright white
    + Walking environment: Flash red -> help car detect pedestrian
    + Indoor reading light: Warm yellow
    + Relax: Dim yellow
    + Party: Disco ball lol
- Transmit signals from the remote to switch between colors or moods.